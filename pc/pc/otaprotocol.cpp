#include "otaprotocol.h"

#include <QStringList>

namespace Ota {

quint16 crc16Modbus(const QByteArray &data)
{
    quint16 crc = 0xFFFF;
    for (int i = 0; i < data.size(); ++i) {
        crc ^= static_cast<quint8>(data.at(i));
        for (int bit = 0; bit < 8; ++bit) {
            if (crc & 0x0001)
                crc = static_cast<quint16>((crc >> 1) ^ 0xA001);
            else
                crc = static_cast<quint16>(crc >> 1);
        }
    }
    return crc;
}

QByteArray buildFrame(quint8 cmd, const QByteArray &payload)
{
    Q_ASSERT(payload.size() <= 255);

    QByteArray frame;
    frame.reserve(payload.size() + 6);
    frame.append(static_cast<char>(kHead));
    frame.append(static_cast<char>(kAddr));
    frame.append(static_cast<char>(cmd));
    frame.append(static_cast<char>(payload.size() & 0xFF));
    frame.append(payload);

    const quint16 crc = crc16Modbus(frame);
    frame.append(static_cast<char>(crc & 0xFF));
    frame.append(static_cast<char>((crc >> 8) & 0xFF));
    return frame;
}

/*
 * BEGIN(0x03) 使用专用帧，固件不读取 len 字段，而是直接读 frame[3..6]:
 *   AA 01 03 size_lo size_hi crc_lo crc_hi frame_crc_lo frame_crc_hi
 */
QByteArray buildBeginFrame(quint16 totalSize, quint16 targetCrc)
{
    QByteArray frame;
    frame.reserve(9);
    frame.append(static_cast<char>(kHead));
    frame.append(static_cast<char>(kAddr));
    frame.append(static_cast<char>(CmdBeginTransmit));
    frame.append(static_cast<char>(totalSize & 0xFF));
    frame.append(static_cast<char>((totalSize >> 8) & 0xFF));
    frame.append(static_cast<char>(targetCrc & 0xFF));
    frame.append(static_cast<char>((targetCrc >> 8) & 0xFF));

    const quint16 crc = crc16Modbus(frame);
    frame.append(static_cast<char>(crc & 0xFF));
    frame.append(static_cast<char>((crc >> 8) & 0xFF));
    return frame;
}

QString statusText(int status)
{
    switch (status) {
    case 0x00: return QStringLiteral("OK");
    case 0x01: return QStringLiteral("帧格式/CRC 错误");
    case 0x02: return QStringLiteral("未知命令");
    case 0x03: return QStringLiteral("参数错误");
    case 0x04: return QStringLiteral("Flash 擦除失败");
    case 0x05: return QStringLiteral("Flash 写入失败");
    case 0x06: return QStringLiteral("状态/偏移错误");
    case 0x07: return QStringLiteral("校验失败");
    case 0x08: return QStringLiteral("地址/长度越界");
    default:   return QStringLiteral("未知状态 0x%1")
                          .arg(static_cast<quint8>(status), 2, 16, QLatin1Char('0')).toUpper();
    }
}

QString hexString(const QByteArray &data)
{
    QStringList parts;
    parts.reserve(data.size());
    for (int i = 0; i < data.size(); ++i)
        parts.append(QStringLiteral("%1")
                         .arg(static_cast<quint8>(data.at(i)), 2, 16, QLatin1Char('0'))
                         .toUpper());
    return parts.join(QLatin1Char(' '));
}

QString versionString(const QByteArray &payload)
{
    if (payload.size() >= 3) {
        return QStringLiteral("V%1.%2.%3")
            .arg(static_cast<quint8>(payload.at(0)))
            .arg(static_cast<quint8>(payload.at(1)))
            .arg(static_cast<quint8>(payload.at(2)));
    }
    return hexString(payload);
}

QString crcHex(quint16 crc)
{
    return QStringLiteral("0x%1")
        .arg(QString::number(crc, 16).rightJustified(4, QLatin1Char('0')).toUpper());
}

} // namespace Ota
