#include "ingest.h"

Ingest::Ingest(QNetworkInterface interface, QHostAddress address, quint32 port, QString mediaDir, QString fileName)
    : m_interface(interface), m_address(address), m_port(port), m_media_dir(mediaDir)
{
    qDebug() << __PRETTY_FUNCTION__ << " " << __LINE__;

    if(fileName.isEmpty())
        m_file_name = mediaDir + "/" + address.toString() + ".ts";
    else
        m_file_name = mediaDir + "/ " + fileName;

}

Ingest::~Ingest()
{

}

bool Ingest::start()
{
    m_file.setFileName(m_file_name);
    if (!m_file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        qCritical() << "unable to open file for writing:" << m_file_name;
       return false;
    }

    if (!m_socket.bind(m_address, m_port, QAbstractSocket::ShareAddress | QAbstractSocket::ReuseAddressHint))
    {
        qCritical() << "unable to bind socket:" << m_address << " " << m_port;
        return false;
    }

    bool joined = false;
    if (m_interface.isValid())
        joined = m_socket.joinMulticastGroup(m_address, m_interface);
    else
        joined = m_socket.joinMulticastGroup(m_address);

    if (!joined)
    {
        qCritical() << "unable to join group:" << m_address;
        return false;
    }

    m_socket.setSocketOption(QAbstractSocket::MulticastTtlOption, 15);
    m_socket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, 1);
    m_socket.setSocketOption(QAbstractSocket::TypeOfServiceOption, 160);
    m_socket.setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1048576 /* 1024 * 1024 */);

    QDataStream stream(&m_file);
    connect(&m_socket, &QUdpSocket::readyRead, [this, &stream]
    {
        while (m_socket.hasPendingDatagrams())
        {
                QByteArray datagram;
                datagram.resize(m_socket.pendingDatagramSize());

                m_socket.readDatagram(datagram.data(), datagram.size());

                if (datagram.size() == RTP_PACKET)
                    stream.writeRawData(datagram.data() + 12, datagram.size() - 12);
                //supports ffmpeg variable packet ize
                else
                    stream.writeRawData(datagram.data(), datagram.size());

        }
    });

    return true;
}
