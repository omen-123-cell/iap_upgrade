#!/usr/bin/env python3
"""
Send a firmware image to the bootloader in fixed-size READ_FIRM (0x04) frames.

Frame (all little-endian):
    AA | 01 | 04 | len | data[len] | crc_lo | crc_hi

len is the number of data bytes (0x80 = 128).
CRC16/Modbus (poly 0xA001, init 0xFFFF) is calculated over the whole frame
except the last two CRC bytes (AA 01 04 len data...), low byte sent first.

The bootloader answers every frame with:
    AA | 01 | 84 | 01 | status | crc_lo | crc_hi

Usage:
    pip install pyserial
    python send_chunks.py app.bin --port COM6
    python send_chunks.py app.bin --port COM6 --baud 115200
    python send_chunks.py app.bin --port COM6 --chunk 128 --no-ack
    python send_chunks.py app.bin --port COM6 --no-pad

Notes:
  * Before sending data, the bootloader must have been put in upgrade state
    (BEGIN_TRANSMIT_FIRM sent, application sector erased). Otherwise each
    frame is answered with status 0x05 (flash write error).
  * By default a short last chunk is padded with 0xFF up to the chunk size,
    so every frame is exactly `--chunk` bytes and len is always 0x80.
    Use --no-pad to send the real remainder instead.
"""

import argparse
import struct
import sys
import time

try:
    import serial
except ImportError:
    sys.exit("pyserial is required: pip install pyserial")

PROTO_HEAD = 0xAA
PROTO_ADDR = 0x01
CMD_DATA = 0x04
ACK_DATA = CMD_DATA | 0x80

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


def hexstr(data) -> str:
    return " ".join(f"{b:02X}" for b in data)


def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc & 0xFFFF


def make_frame(cmd: int, payload: bytes) -> bytes:
    if len(payload) > 255:
        raise ValueError("payload does not fit in one frame")
    frame = bytes([PROTO_HEAD, PROTO_ADDR, cmd, len(payload)]) + payload
    return frame + struct.pack("<H", crc16(frame))


def read_ack(ser: serial.Serial, timeout: float):
    """Return (cmd, status) of the first valid response frame."""
    deadline = time.time() + timeout
    buf = bytearray()
    while time.time() < deadline:
        b = ser.read(1)
        if not b:
            continue
        buf += b

        i = buf.find(bytes([PROTO_HEAD, PROTO_ADDR]))
        if i < 0:
            # keep at most the last byte, it could be the start of a header
            if len(buf) > 2:
                del buf[:-1]
            continue
        if i > 0:
            del buf[:i]
        if len(buf) < 6:
            continue

        total = 6 + buf[3]
        if len(buf) < total:
            continue

        f = bytes(buf[:total])
        del buf[:total]
        if crc16(f[:-2]) != (f[-2] | (f[-1] << 8)):
            continue
        return f[2], f[4]

    raise TimeoutError("no response from the device")


def main() -> int:
    ap = argparse.ArgumentParser(
        description="Send a firmware image in READ_FIRM (0x04) frames")
    ap.add_argument("image", nargs="?", default="app.bin",
                    help="firmware file (default: app.bin)")
    ap.add_argument("--port", default="COM6", help="serial port (default: COM6)")
    ap.add_argument("--baud", type=int, default=9600, help="baud rate (default: 9600)")
    ap.add_argument("--chunk", type=int, default=128,
                    help="data bytes per frame (default: 128 = 0x80)")
    ap.add_argument("--no-ack", action="store_true",
                    help="do not wait for the response after each frame")
    ap.add_argument("--no-pad", action="store_true",
                    help="send the real remainder in the last frame instead of 0xFF padding")
    ap.add_argument("--timeout", type=float, default=2.0,
                    help="response timeout per frame in seconds (default: 2.0)")
    ap.add_argument("--delay", type=float, default=0.0,
                    help="delay between frames in seconds (default: 0)")
    args = ap.parse_args()

    if not (1 <= args.chunk <= 255):
        ap.error("--chunk must be between 1 and 255")

    try:
        with open(args.image, "rb") as fp:
            image = fp.read()
    except OSError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    if not image:
        print("error: image is empty", file=sys.stderr)
        return 1

    original_size = len(image)
    if args.no_pad and (original_size % args.chunk):
        # FLASH_Write_Buffer programs whole words; warn about the tail
        tail = original_size % args.chunk
        if tail % 4:
            print(f"warning: last chunk is {tail} bytes (not a multiple of 4); "
                  "the device only programs complete 32-bit words", file=sys.stderr)

    if not args.no_pad and (original_size % args.chunk):
        image += b"\xFF" * (args.chunk - original_size % args.chunk)

    frames = [image[i:i + args.chunk] for i in range(0, len(image), args.chunk)]
    print(f"image  : {args.image}")
    print(f"size   : {original_size} bytes" +
          (f" (padded to {len(image)} with 0xFF)" if len(image) != original_size else ""))
    print(f"frames : {len(frames)} x {args.chunk} bytes")
    print(f"port   : {args.port} @ {args.baud} 8N1"
          f"{' (no ACK)' if args.no_ack else ''}")
    print()

    print("first frame:", hexstr(make_frame(CMD_DATA, frames[0])))
    if len(frames) > 1:
        print("last  frame:", hexstr(make_frame(CMD_DATA, frames[-1])))
    print()

    try:
        with serial.Serial(args.port, args.baud, timeout=0.05) as ser:
            ser.reset_input_buffer()
            t0 = time.time()

            for idx, part in enumerate(frames, 1):
                ser.write(make_frame(CMD_DATA, part))
                ser.flush()

                if not args.no_ack:
                    cmd, status = read_ack(ser, args.timeout)
                    if cmd != ACK_DATA:
                        print(f"\nframe {idx}: unexpected response cmd "
                              f"0x{cmd:02X}", file=sys.stderr)
                        return 1
                    if status != 0:
                        print(f"\nframe {idx} (@0x{(idx - 1) * args.chunk:04X}): "
                              f"{STATUS.get(status, f'0x{status:02X}')}",
                              file=sys.stderr)
                        return 1

                if args.delay:
                    time.sleep(args.delay)

                if idx % 16 == 0 or idx == len(frames):
                    sent = min(idx * args.chunk, len(image))
                    print(f"\r  sent {sent}/{len(image)} bytes "
                          f"({sent * 100 // len(image)}%)", end="", flush=True)

            print(f"\n  done in {time.time() - t0:.1f}s")
    except (TimeoutError, serial.SerialException) as exc:
        print(f"\nerror: {exc}", file=sys.stderr)
        return 1

    print("done")
    return 0


if __name__ == "__main__":
    sys.exit(main())
