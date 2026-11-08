#ifndef OTAPROTOCOL_H
#define OTAPROTOCOL_H

#include <QByteArray>
#include <QString>
#include <QtGlobal>

/*
 * Bootloader 串口 IAP 协议（与 Bootloader/bootloader/protocol 保持一致）
 *
 * 帧格式（多字节字段小端）:
 *   AA | addr | cmd | len | data[len] | crc_lo | crc_hi
 *
 * CRC16/Modbus（poly 0xA001，init 0xFFFF）的计算范围是整帧去掉最后两个
 * CRC 字节，包含 0xAA 帧头。
 *
 * 应答帧的命令为请求命令 | 0x80，len = 1 + 数据长度，
 * data[0] 为状态码，其后为可选数据。
 */
namespace Ota {

const quint8 kHead = 0xAA;
const quint8 kAddr = 0x01;

enum Command {
    CmdWaitUpgrade   = 0x01,
    CmdBeginTransmit = 0x03,
    CmdReadFirm      = 0x04,
    CmdEndTransmit   = 0x05,
    CmdGetVersion    = 0x06,
    CmdConvertToApp  = 0x07,
    AckFlag          = 0x80
};

enum LogLevel {
    LogInfo = 0,
    LogSuccess,
    LogWarning,
    LogError
};

quint16 crc16Modbus(const QByteArray &data);
QByteArray buildFrame(quint8 cmd, const QByteArray &payload = QByteArray());
QByteArray buildBeginFrame(quint16 totalSize, quint16 targetCrc);
QString statusText(int status);
QString hexString(const QByteArray &data);
QString versionString(const QByteArray &payload);
QString crcHex(quint16 crc);

} // namespace Ota

#endif // OTAPROTOCOL_H
