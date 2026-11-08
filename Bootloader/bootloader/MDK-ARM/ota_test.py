#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Bootloader 串口 IAP 升级测试脚本

配合本工程的 protocol/protocol.c 与 Core/Src/usart.c 使用。

用法:
    python ota_test.py                                   # 默认 COM6 / 9600 / app.bin
    python ota_test.py -p COM7 -f app.bin
    python ota_test.py -p COM6 -f app.bin -c 128 --delay 0.005

流程:
    1. WAIT_UPGRADE        (0x01) 握手
    2. BEGIN_TRANSMIT_FIRM (0x03) 发送总长度 + 目标 CRC，固件擦除 sector 4~6
    3. READ_FIRM           (0x04) 分包发送数据，每帧等应答
    4. END_TRANSMIT_FIRM   (0x05) 固件校验 CRC 后跳转到 0x08010000

帧格式 (多字节小端):
    AA | addr | cmd | len | data[len] | crc_lo | crc_hi

与固件实现对齐的几个要点:
    * CRC16/Modbus (poly 0xA001, init 0xFFFF) 的范围包含 0xAA 帧头，
      即“整帧去掉最后两个 CRC 字节”。protocol.h 的注释写的是 addr..data，
      与 protocol.c 的实现不一致，这里以代码为准。
    * BEGIN(0x03) 帧没有独立的 len 字段，protocol.c 直接读 frame[3..6]：
          AA 01 03 size_lo size_hi crc_lo crc_hi frame_crc_lo frame_crc_hi
    * 固件用 uint16_t 记录长度，所以单次升级不能超过 65535 字节。
    * FLASH_Write_Buffer() 按 32 位字编程，每帧数据长度必须是 4 的倍数；
      文件末尾用 0xFF 补齐，目标 CRC 也按补齐后的数据计算。
    * 协议没有 offset 字段，固件按顺序追加写入。某帧超时后重发会让后续数据
      错位，所以脚本一旦超时就直接退出；复位 Bootloader 后重跑即可，
      BEGIN 会重新擦除 App 区。
"""

import argparse
import os
import struct
import sys
import time

try:
    import serial
except ImportError:
    sys.exit("缺少 pyserial，请先执行: pip install pyserial")


# ---------------- 协议常量 (与 protocol.h 一致) ----------------
PROTO_HEAD = 0xAA
PROTO_ADDR = 0x01

WAIT_UPGRADE        = 0x01
BEGIN_TRANSMIT_FIRM = 0x03
READ_FIRM           = 0x04
END_TRANSMIT_FIRM   = 0x05
GET_FIRM_VERSION    = 0x06
ACK_FLAG            = 0x80

# flash_if.h 中的 App 区
FLASH_APP_ADDR = 0x08010000
SIZE_RUN       = 256 * 1024

# protocol.c 用 uint16_t 记录 total_size / firm_size
MAX_IMAGE_SIZE = 0xFFFF

STATUS_TEXT = {
    0x00: "OK",
    0x01: "帧格式/CRC 错误",
    0x02: "未知命令",
    0x03: "参数错误",
    0x04: "Flash 擦除失败",
    0x05: "Flash 写入失败",
    0x06: "状态/偏移错误",
    0x07: "校验失败",
    0x08: "地址/长度越界",
}


class OtaError(Exception):
    """协议交互失败"""


def status_text(status):
    return STATUS_TEXT.get(status, "未知状态 0x%02X" % status)


def crc16_modbus(data):
    """CRC16/Modbus: poly 0xA001, init 0xFFFF"""
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc & 0xFFFF


def hexstr(data):
    return " ".join("%02X" % b for b in data)


def build_frame(cmd, payload=b""):
    """普通帧: AA | addr | cmd | len | data | crc_lo crc_hi"""
    if len(payload) > 255:
        raise ValueError("单帧数据不能超过 255 字节")
    frame = bytes([PROTO_HEAD, PROTO_ADDR, cmd, len(payload)]) + payload
    return frame + struct.pack("<H", crc16_modbus(frame))


def build_begin_frame(total_size, target_crc):
    """
    BEGIN(0x03) 专用帧。

    protocol.c 没有使用 len 字段，而是直接读:
        total_size = frame[3] | frame[4] << 8
        target_crc = frame[5] | frame[6] << 8
    所以这一帧是 9 字节:
        AA 01 03 size_lo size_hi crc_lo crc_hi frame_crc_lo frame_crc_hi
    """
    frame = bytes([PROTO_HEAD, PROTO_ADDR, BEGIN_TRANSMIT_FIRM,
                   total_size & 0xFF, (total_size >> 8) & 0xFF,
                   target_crc & 0xFF, (target_crc >> 8) & 0xFF])
    return frame + struct.pack("<H", crc16_modbus(frame))


def read_response(ser, timeout):
    """
    等待并解析一帧应答。
    返回 (cmd, status, data)，超时返回 None。
    应答帧: AA 01 (cmd|0x80) len status data... crc_lo crc_hi
    """
    deadline = time.time() + timeout
    buf = bytearray()

    while time.time() < deadline:
        b = ser.read(1)
        if not b:
            continue
        buf += b

        # 从缓冲区里找一帧完整且 CRC 正确的应答
        while True:
            i = buf.find(bytes([PROTO_HEAD, PROTO_ADDR]))
            if i < 0:
                if len(buf) > 1:
                    del buf[:-1]
                break
            if i > 0:
                del buf[:i]
            if len(buf) < 4:
                break
            total = 6 + buf[3]          # 4 字节头 + len + 2 字节 CRC
            if len(buf) < total:
                break
            frame = bytes(buf[:total])
            if crc16_modbus(frame[:-2]) != (frame[-2] | (frame[-1] << 8)):
                del buf[0]              # CRC 不对，丢掉一个字节重新找帧头
                continue
            return frame[2], frame[4], frame[5:-2]

    return None


def request(ser, frame, cmd, timeout, label):
    """发送一帧并等待对应命令的应答，返回 (status, data)"""
    ser.reset_input_buffer()
    ser.write(frame)
    ser.flush()

    resp = read_response(ser, timeout)
    if resp is None:
        raise OtaError("%s: %.1fs 内没有收到应答" % (label, timeout))

    rcmd, status, data = resp
    if rcmd != (cmd | ACK_FLAG):
        raise OtaError("%s: 应答命令 0x%02X，预期 0x%02X"
                       % (label, rcmd, cmd | ACK_FLAG))
    return status, data


def check_image_layout(raw):
    """
    检查 bin 的向量表是否与 flash_if.h 的 ADDR_RUN 匹配。
    返回 True/False，只提示不阻止升级（可以先验证传输和 CRC）。
    """
    if len(raw) < 8:
        return True

    initial_sp, reset_vector = struct.unpack_from("<II", raw, 0)
    ok = True

    if not (FLASH_APP_ADDR <= reset_vector < FLASH_APP_ADDR + SIZE_RUN):
        print("[提示] bin 复位向量 = 0x%08X，不在 App 区 0x%08X~0x%08X。"
              % (reset_vector, FLASH_APP_ADDR, FLASH_APP_ADDR + SIZE_RUN - 1))
        print("       说明 App 工程仍按 0x08000000 链接：升级和 CRC 校验会成功，")
        print("       但跳转后不会运行这个 App。请把 App 工程 IROM1 起始地址改为")
        print("       0x08010000（大小 0x00040000）后重新编译。")
        ok = False

    # 与 Jump_To_RunArea() 里的检查保持一致
    if (initial_sp & 0x2FFE0000) != 0x20000000:
        print("[提示] bin 初始栈指针 = 0x%08X，不满足 Jump_To_RunArea() 的检查，"
              "可能不会跳转。" % initial_sp)
        ok = False

    if ok:
        print("[检查] bin 向量表位于 App 区，链接地址正确。")
    return ok


def parse_args():
    ap = argparse.ArgumentParser(description="Bootloader 串口 IAP 升级测试")
    ap.add_argument("-p", "--port", default="COM6",
                    help="串口，默认 COM6")
    ap.add_argument("-b", "--baud", type=int, default=9600,
                    help="波特率，默认 9600（与 usart.c 一致）")
    ap.add_argument("-f", "--file", default="app.bin",
                    help="固件 bin 路径，默认当前目录 app.bin")
    ap.add_argument("-c", "--chunk", type=int, default=128,
                    help="每帧数据字节数，4 的倍数，4~250，默认 128")
    ap.add_argument("-t", "--timeout", type=float, default=2.0,
                    help="普通应答超时秒数，默认 2.0")
    ap.add_argument("--erase-timeout", type=float, default=15.0,
                    help="BEGIN 擦除应答超时秒数，默认 15.0")
    ap.add_argument("--delay", type=float, default=0.0,
                    help="两帧之间的额外延时秒数，默认 0")
    return ap.parse_args()


def main():
    args = parse_args()

    if args.chunk < 4 or args.chunk > 250 or args.chunk % 4 != 0:
        print("[错误] --chunk 必须是 4~250 之间且为 4 的倍数")
        return 2

    # ---------- 读取固件 ----------
    try:
        with open(args.file, "rb") as fp:
            raw = fp.read()
    except OSError as exc:
        print("[错误] 打不开固件文件: %s" % exc)
        return 1

    if not raw:
        print("[错误] 固件文件是空的")
        return 1

    pad = (-len(raw)) % 4
    image = raw + (b"\xFF" * pad)
    if len(image) > MAX_IMAGE_SIZE:
        print("[错误] 固件 %d 字节，超过固件端 uint16_t 计数上限 %d 字节"
              % (len(image), MAX_IMAGE_SIZE))
        return 1

    fw_crc = crc16_modbus(image)
    frames = (len(image) + args.chunk - 1) // args.chunk
    est = frames * (args.chunk + 13) * 10.0 / args.baud

    print("================ IAP 升级 ================")
    print("固件文件 : %s" % os.path.abspath(args.file))
    print("原始大小 : %d 字节" % len(raw))
    print("发送大小 : %d 字节%s" % (len(image),
          ("（末尾补 %d 个 0xFF 凑齐 4 字节）" % pad) if pad else ""))
    print("目标 CRC : 0x%04X（按发送内容计算）" % fw_crc)
    print("分包     : %d 帧 x %d 字节" % (frames, args.chunk))
    print("串口     : %s @ %d 8N1" % (args.port, args.baud))
    print("预计耗时 : 约 %.1f s（不含 Flash 擦除）" % est)
    print("==========================================")

    layout_ok = check_image_layout(raw)

    # ---------- 打开串口 ----------
    try:
        ser = serial.Serial(args.port, args.baud, timeout=0.05)
    except serial.SerialException as exc:
        print("[错误] 打开串口 %s 失败: %s" % (args.port, exc))
        try:
            from serial.tools import list_ports
            ports = [p.device for p in list_ports.comports()]
            if ports:
                print("       当前可用串口: %s" % ", ".join(ports))
        except Exception:
            pass
        return 1

    start_time = time.time()

    with ser:
        time.sleep(0.2)
        ser.reset_input_buffer()

        try:
            # ---------- 1. 握手 ----------
            print("[1/4] 握手 (WAIT_UPGRADE) ...")
            status, _ = request(ser, build_frame(WAIT_UPGRADE),
                                WAIT_UPGRADE, args.timeout, "WAIT_UPGRADE")
            if status != 0:
                print("[错误] 握手失败: %s" % status_text(status))
                return 1
            print("      OK")

            # 版本查询失败不影响升级（老固件可能没有这条命令）
            try:
                status, data = request(ser, build_frame(GET_FIRM_VERSION),
                                       GET_FIRM_VERSION, args.timeout,
                                       "GET_FIRM_VERSION")
                if status == 0 and data:
                    print("      Bootloader 版本信息: %s" % hexstr(data))
                else:
                    print("      版本查询返回: %s" % status_text(status))
            except OtaError as exc:
                print("      [警告] %s" % exc)

            # ---------- 2. BEGIN：擦除 + 记录长度/CRC ----------
            print("[2/4] 开始传输 (BEGIN_TRANSMIT_FIRM)，擦除 App 区 ...")
            try:
                status, _ = request(
                    ser, build_begin_frame(len(image), fw_crc),
                    BEGIN_TRANSMIT_FIRM, args.erase_timeout,
                    "BEGIN_TRANSMIT_FIRM")
            except OtaError as exc:
                print("[错误] %s" % exc)
                print("       固件只在擦除成功时才回应，超时也可能是 Flash 擦除失败。")
                return 1
            if status != 0:
                print("[错误] 擦除失败: %s" % status_text(status))
                return 1
            print("      OK（Flash 已擦除）")

            # ---------- 3. 分包发送 ----------
            print("[3/4] 发送固件数据 ...")
            offset = 0
            packet = 0
            t0 = time.time()

            while offset < len(image):
                part = image[offset:offset + args.chunk]
                try:
                    status, _ = request(
                        ser, build_frame(READ_FIRM, part), READ_FIRM,
                        args.timeout, "第 %d 帧 @0x%04X" % (packet + 1, offset))
                except OtaError as exc:
                    print("\n[错误] %s" % exc)
                    print("       协议没有 offset 字段，不能安全重发；请复位 Bootloader")
                    print("       后重新运行本脚本（BEGIN 会重新擦除 App 区）。")
                    return 1

                if status != 0:
                    print("\n[错误] 第 %d 帧 (偏移 0x%04X) 写入失败: %s"
                          % (packet + 1, offset, status_text(status)))
                    return 1

                offset += len(part)
                packet += 1

                if args.delay:
                    time.sleep(args.delay)

                elapsed = time.time() - t0
                rate = offset / elapsed if elapsed > 0 else 0.0
                print("\r      %d/%d 字节 (%d%%)  %.2f KB/s"
                      % (offset, len(image), offset * 100 // len(image),
                         rate / 1024.0), end="", flush=True)

            print()
            print("      OK（共 %d 帧）" % packet)

            # ---------- 4. END：校验并跳转 ----------
            print("[4/4] 结束传输 (END_TRANSMIT_FIRM)，校验 CRC ...")
            try:
                status, _ = request(ser, build_frame(END_TRANSMIT_FIRM),
                                    END_TRANSMIT_FIRM, 5.0,
                                    "END_TRANSMIT_FIRM")
            except OtaError as exc:
                print("[错误] %s" % exc)
                print("       固件算出的 CRC 与目标 CRC 0x%04X 不一致，"
                      "或者设备没有回应。" % fw_crc)
                return 1

            if status != 0:
                print("[错误] 结束传输失败: %s" % status_text(status))
                return 1
            print("      OK，CRC 校验通过，单片机正在跳转到 App")

        except serial.SerialException as exc:
            print("\n[错误] 串口异常: %s" % exc)
            return 1

    elapsed = time.time() - start_time
    print()
    print("升级完成: %d 字节, CRC=0x%04X, 用时 %.1f s"
          % (len(image), fw_crc, elapsed))

    if not layout_ok:
        print("[提示] 再次提醒：该 bin 的链接地址不是 0x08010000，"
              "跳转后不会运行，请重新编译 App。")

    return 0


if __name__ == "__main__":
    sys.exit(main())
