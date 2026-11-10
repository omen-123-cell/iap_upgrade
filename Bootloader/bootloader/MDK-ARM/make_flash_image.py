#!/usr/bin/env python3
"""
Build one Intel HEX image containing:
  - the bootloader at 0x08000000..0x0800FFFF (64 KB boot region)
  - the run-linked application image placed in the storage area at 0x08060000

The app binary must be linked for the run area (0x08010000) and is only
*stored* at 0x08060000; the bootloader copies it back to 0x08010000.

Usage (run from this folder):
    python make_flash_image.py
    python make_flash_image.py --boot-hex bootloader/bootloader.hex \
        --app-bin ../../../app/app/MDK-ARM/app/app.bin --out flash_image.hex
"""

import argparse
import sys
from pathlib import Path

BOOT_START = 0x08000000
BOOT_END = 0x08010000       # 64 KB: sectors 0..3

RUN_START = 0x08010000      # ADDR_RUN
RUN_END = 0x08050000        # ADDR_RUN + SIZE_RUN (256 KB)

STORAGE_START = 0x08060000  # ADDR_MEMORY
STORAGE_END = 0x08080000    # sector 7 (128 KB)

HERE = Path(__file__).resolve().parent
DEFAULT_BOOT_HEX = HERE / "bootloader" / "bootloader.hex"
DEFAULT_APP_BIN = HERE.parents[2] / "app" / "app" / "MDK-ARM" / "app" / "app.bin"
DEFAULT_OUT = HERE / "flash_image.hex"


def parse_ihex(path):
    """Parse an Intel HEX file into {address: byte}."""
    mem = {}
    ext = 0
    with path.open("r", encoding="ascii", errors="strict") as f:
        for lineno, raw in enumerate(f, 1):
            line = raw.strip()
            if not line:
                continue
            if not line.startswith(":"):
                raise ValueError(f"{path}:{lineno}: record must start with ':'")
            try:
                rec = bytes.fromhex(line[1:])
            except ValueError:
                raise ValueError(f"{path}:{lineno}: invalid hex digits")
            if len(rec) < 5:
                raise ValueError(f"{path}:{lineno}: record too short")

            count = rec[0]
            if len(rec) != count + 5:
                raise ValueError(f"{path}:{lineno}: length does not match byte count")
            if sum(rec) & 0xFF:
                raise ValueError(f"{path}:{lineno}: bad checksum")

            addr = (rec[1] << 8) | rec[2]
            rtype = rec[3]
            data = rec[4:4 + count]

            if rtype == 0x00:          # data
                for off, value in enumerate(data):
                    a = ext + addr + off
                    if a in mem:
                        raise ValueError(
                            f"{path}:{lineno}: duplicate address 0x{a:08X}")
                    mem[a] = value
            elif rtype == 0x01:        # EOF
                break
            elif rtype == 0x02:        # extended segment address
                if count != 2:
                    raise ValueError(f"{path}:{lineno}: bad type-02 record")
                ext = ((data[0] << 8) | data[1]) << 4
            elif rtype == 0x04:        # extended linear address
                if count != 2:
                    raise ValueError(f"{path}:{lineno}: bad type-04 record")
                ext = ((data[0] << 8) | data[1]) << 16
            elif rtype in (0x03, 0x05):  # start address records: ignored
                continue
            else:
                raise ValueError(f"{path}:{lineno}: unsupported record type {rtype}")
    return mem


def addr_ranges(mem):
    """Return [(start, end), ...] for contiguous runs of addresses."""
    result = []
    start = prev = None
    for a in sorted(mem):
        if start is None:
            start = prev = a
        elif a == prev + 1:
            prev = a
        else:
            result.append((start, prev + 1))
            start = prev = a
    if start is not None:
        result.append((start, prev + 1))
    return result


def ihex_record(addr, rtype, data):
    payload = bytes([len(data), (addr >> 8) & 0xFF, addr & 0xFF, rtype]) + data
    checksum = (-sum(payload)) & 0xFF
    return ":" + (payload + bytes([checksum])).hex().upper()


def write_ihex(mem, path):
    lines = []
    upper = None
    for start, end in addr_ranges(mem):
        pos = start
        while pos < end:
            up = pos >> 16
            if up != upper:
                upper = up
                lines.append(ihex_record(0, 0x04,
                                         bytes([(up >> 8) & 0xFF, up & 0xFF])))
            chunk_end = min(pos + 16, end, ((pos >> 16) + 1) << 16)
            lines.append(ihex_record(pos & 0xFFFF, 0x00,
                                     bytes(mem[a] for a in range(pos, chunk_end))))
            pos = chunk_end
    lines.append(":00000001FF")
    with path.open("w", encoding="ascii", newline="\n") as f:
        f.write("\n".join(lines) + "\n")


def print_ranges(mem):
    for start, end in addr_ranges(mem):
        print(f"  0x{start:08X} - 0x{end - 1:08X}  ({end - start} bytes)")


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--boot-hex", type=Path, default=DEFAULT_BOOT_HEX,
                    help="bootloader Intel HEX (default: %(default)s)")
    img = ap.add_mutually_exclusive_group()
    img.add_argument("--storage-bin", "--app-bin", dest="app_bin", type=Path,
                     default=DEFAULT_APP_BIN,
                     help="raw image linked for the run area, placed in the storage "
                          "area (default: %(default)s)")
    img.add_argument("--storage-hex", dest="storage_hex", type=Path,
                     help="Intel HEX linked for the run area; addresses are "
                          "rebased from 0x08010000 to 0x08060000")
    ap.add_argument("--app-addr", type=lambda s: int(s, 0), default=STORAGE_START,
                    help="where to place the app image (default: 0x08060000)")
    ap.add_argument("--out", type=Path, default=DEFAULT_OUT,
                    help="output Intel HEX (default: %(default)s)")
    args = ap.parse_args()

    boot_hex = args.boot_hex.resolve()
    out = args.out.resolve()

    if not boot_hex.is_file():
        print(f"ERROR: bootloader hex not found: {boot_hex}")
        return 1

    try:
        boot = parse_ihex(boot_hex)
    except ValueError as exc:
        print(f"ERROR: {exc}")
        return 1

    print(f"Bootloader : {boot_hex}")
    print("  ranges:")
    print_ranges(boot)

    bad = [(s, e) for s, e in addr_ranges(boot) if s < BOOT_START or e > BOOT_END]
    if bad:
        print("ERROR: bootloader image has data outside the 64 KB boot region "
              f"0x{BOOT_START:08X}..0x{BOOT_END - 1:08X}:")
        for s, e in bad:
            print(f"  0x{s:08X} - 0x{e - 1:08X}  ({e - s} bytes)")
        print("The Keil target probably still has IROM2 enabled at 0x08060000.")
        print("Remove IROM2 in Options for Target -> Target tab, rebuild, "
              "then run this script again.")
        return 2

    combined = dict(boot)

    if args.storage_hex:
        storage_hex = args.storage_hex.resolve()
        if not storage_hex.is_file():
            print(f"ERROR: storage hex not found: {storage_hex}")
            return 1
        try:
            storage = parse_ihex(storage_hex)
        except ValueError as exc:
            print(f"ERROR: {exc}")
            return 1

        print(f"Storage hex: {storage_hex}")
        print("  ranges:")
        print_ranges(storage)

        bad = [(s, e) for s, e in addr_ranges(storage)
               if s < RUN_START or e > RUN_END]
        if bad:
            print("ERROR: storage image is not linked for the run area "
                  f"0x{RUN_START:08X}..0x{RUN_END - 1:08X}:")
            for s, e in bad:
                print(f"  0x{s:08X} - 0x{e - 1:08X}  ({e - s} bytes)")
            print("Rebuild the storage program with IROM1 start = 0x08010000, "
                  "then run this script again.")
            return 2

        if RUN_START not in storage:
            print(f"ERROR: storage hex has no vector table at 0x{RUN_START:08X}")
            return 1

        size = max(storage) + 1 - RUN_START
        if size > STORAGE_END - STORAGE_START:
            print(f"ERROR: storage image is {size} bytes, larger than the storage "
                  f"area 0x{STORAGE_START:08X}..0x{STORAGE_END - 1:08X}")
            return 1

        sp = int.from_bytes(
            bytes(storage[RUN_START + i] for i in range(4)), "little")
        reset = int.from_bytes(
            bytes(storage[RUN_START + 4 + i] for i in range(4)), "little")
        if not (0x20000000 <= sp <= 0x20020000):
            print(f"WARNING: initial SP 0x{sp:08X} is outside SRAM "
                  "0x20000000..0x20020000")
        if not (RUN_START <= (reset & ~1) < RUN_END) or not (reset & 1):
            print(f"WARNING: reset vector 0x{reset:08X} does not look like an "
                  f"image linked for the run area 0x{RUN_START:08X}..0x{RUN_END - 1:08X}")

        for a, value in storage.items():
            combined[STORAGE_START + (a - RUN_START)] = value
    else:
        app_bin = args.app_bin.resolve()
        if not app_bin.is_file():
            print(f"ERROR: storage binary not found: {app_bin}")
            return 1

        app = app_bin.read_bytes()
        print(f"Storage image: {app_bin}  ({len(app)} bytes)")

        if len(app) == 0:
            print("ERROR: app binary is empty")
            return 1
        if args.app_addr < STORAGE_START or args.app_addr + len(app) > STORAGE_END:
            print(f"ERROR: {len(app)} bytes at 0x{args.app_addr:08X} do not fit in "
                  f"the storage area 0x{STORAGE_START:08X}..0x{STORAGE_END - 1:08X}")
            return 1
        if args.app_addr != STORAGE_START:
            print(f"WARNING: app is placed at 0x{args.app_addr:08X}, but the "
                  f"bootloader copies from ADDR_MEMORY = 0x{STORAGE_START:08X}")

        if len(app) >= 8:
            sp = int.from_bytes(app[0:4], "little")
            reset = int.from_bytes(app[4:8], "little")
            if not (0x20000000 <= sp <= 0x20020000):
                print(f"WARNING: initial SP 0x{sp:08X} is outside SRAM "
                      "0x20000000..0x20020000")
            if not (RUN_START <= (reset & ~1) < RUN_END) or not (reset & 1):
                print(f"WARNING: reset vector 0x{reset:08X} does not look like an "
                      f"image linked for the run area 0x{RUN_START:08X}..0x{RUN_END - 1:08X}")

        overlap = [a for a in range(args.app_addr, args.app_addr + len(app))
                   if a in boot]
        if overlap:
            print(f"ERROR: app image overlaps bootloader data at 0x{overlap[0]:08X}")
            return 1

        for off, value in enumerate(app):
            combined[args.app_addr + off] = value

    write_ihex(combined, out)
    print(f"\nCombined image written: {out}")
    print("  ranges:")
    print_ranges(combined)
    return 0


if __name__ == "__main__":
    sys.exit(main())
