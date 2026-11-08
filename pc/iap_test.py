#!/usr/bin/env python3
"""
IAP protocol test / firmware download tool.

Usage:
    pip install pyserial
    python iap_test.py COM3 firmware.bin --version 0x0102
    python iap_test.py COM3 firmware.bin --version 0x0102 --jump

Frame: AA 55 | addr | cmd | len | data | crc_lo crc_hi
CRC16/Modbus over addr..data.
"""

import argparse
import struct
import sys
import time

import serial

SOF = b"\xAA\x55"
ADDR = 0x01

CMD_ENTER = 0x01
CMD_BEGIN = 0x02
CMD_DATA = 0x03
CMD_END = 0x04
CMD_VERSION = 0x05
CMD_JUMP = 0x06

CHUNK = 232                 # max payload accepted by the bootloader
STATUS = {
    0x00: "OK",
    0x01: "frame/CRC error",
    0x02: "unknown command",
    0x03: "bad parameter",
    0x04: "flash erase error",
    0x05: "flash write error",
    0x06: "wrong state / offset",
    0x07: "verify error",
    0x08: "out of range",
}


def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


def make_frame(cmd: int, payload: bytes = b"") -> bytes:
    body = bytes([ADDR, cmd, len(payload)]) + payload
    return SOF + body + struct.pack("<H", crc16(body))


def read_frame(ser: serial.Serial, timeout: float):
    """Wait for one valid response frame; return (cmd, status, data)."""
    deadline = time.time() + timeout
    buf = b""
    last = time.time()

    while time.time() < deadline:
        b = ser.read(1)
        if b:
            buf += b
            last = time.time()
        elif buf and (time.time() - last) > 0.05:
            break

    i = 0
    while i + 7 <= len(buf):
        if buf[i:i + 2] != SOF:
            i += 1
            continue

        ln = buf[i + 4]
        total = 7 + ln
        if i + total > len(buf):
            break

        f = buf[i:i + total]
        crc = f[5 + ln] | (f[6 + ln] << 8)
        if crc16(f[2:5 + ln]) == crc:
            return f[3], f[5], f[6:5 + ln]
        i += total

    return None


def transact(ser: serial.Serial, cmd: int, payload: bytes = b"", timeout: float = 1.0):
    ser.reset_input_buffer()
    ser.write(make_frame(cmd, payload))
    ser.flush()

    resp = read_frame(ser, timeout)
    if resp is None:
        raise TimeoutError(f"cmd 0x{cmd:02X}: no response (timeout {timeout}s)")

    rcmd, status, data = resp
    if rcmd != (cmd | 0x80):
        raise RuntimeError(f"cmd 0x{cmd:02X}: unexpected response cmd 0x{rcmd:02X}")

    return status, data


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("port", help="e.g. COM3 or /dev/ttyUSB0")
    ap.add_argument("firmware", help="firmware .bin file")
    ap.add_argument("--version", type=lambda s: int(s, 0), default=0x0100,
                    help="firmware version (default 0x0100)")
    ap.add_argument("--baud", type=int, default=115200)
    ap.add_argument("--jump", action="store_true",
                    help="send CONVERT_TO_APP at the end")
    args = ap.parse_args()

    with open(args.firmware, "rb") as fp:
        image = fp.read()
    if not image:
        print("firmware file is empty", file=sys.stderr)
        return 1

    size = len(image)
    fw_crc = crc16(image)
    print(f"firmware: {args.firmware}  size={size}  crc16=0x{fw_crc:04X}  "
          f"version=0x{args.version:04X}")

    ser = serial.Serial(args.port, args.baud, timeout=0.1)

    try:
        status, _ = transact(ser, CMD_ENTER)
        if status != 0:
            print(f"enter upgrade failed: {STATUS.get(status, status)}")
            return 1
        print("enter upgrade: OK")

        payload = struct.pack("<IHH", size, args.version, fw_crc)
        status, data = transact(ser, CMD_BEGIN, payload, timeout=15.0)
        if status != 0:
            print(f"begin transfer failed: {STATUS.get(status, status)}")
            return 1
        max_chunk = struct.unpack("<H", data)[0] if len(data) >= 2 else CHUNK
        print(f"begin transfer: OK (max chunk {max_chunk})")

        chunk = min(CHUNK, max_chunk)
        offset = 0
        retries = 0
        while offset < size:
            part = image[offset:offset + chunk]
            status, data = transact(ser, CMD_DATA,
                                    struct.pack("<I", offset) + part,
                                    timeout=2.0)
            if status == 0:
                expected = struct.unpack("<I", data)[0] if len(data) >= 4 else offset + len(part)
                if expected != offset + len(part):
                    print(f"offset mismatch: device expects {expected}, got {offset + len(part)}")
                    offset = expected
                else:
                    offset += len(part)
                    retries = 0
                    print(f"\r  sent {offset}/{size} bytes ({offset * 100 // size}%)",
                          end="", flush=True)
            elif status == 0x06 and len(data) >= 4:
                offset = struct.unpack("<I", data)[0]
                print(f"\n  resync to offset {offset}")
                retries += 1
            else:
                print(f"\nwrite chunk @{offset} failed: {STATUS.get(status, status)}")
                return 1

            if retries > 3:
                print("too many retries", file=sys.stderr)
                return 1
        print()

        status, _ = transact(ser, CMD_END, struct.pack("<H", fw_crc), timeout=15.0)
        if status != 0:
            print(f"end transfer failed: {STATUS.get(status, status)}")
            return 1
        print("end transfer: OK (CRC verified)")

        status, data = transact(ser, CMD_VERSION)
        version = struct.unpack("<H", data)[0] if len(data) >= 2 else 0
        print(f"get version: status={STATUS.get(status, status)} version=0x{version:04X}")

        if args.jump:
            status, _ = transact(ser, CMD_JUMP)
            print(f"jump to app: status={STATUS.get(status, status)}")

    except (TimeoutError, RuntimeError) as exc:
        print(f"\nerror: {exc}", file=sys.stderr)
        return 1
    finally:
        ser.close()

    print("done")
    return 0


if __name__ == "__main__":
    sys.exit(main())
