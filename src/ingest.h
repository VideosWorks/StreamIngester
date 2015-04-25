#ifndef INGEST_H
#define INGEST_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QFile>
#include <QReadWriteLock>

enum packet_type { UDP_PACKET = 1316, RTP_PACKET = 1328, TS_SIZE = 188 };
class Ingest : public QObject
{
public:
    Ingest(QNetworkInterface interface, QHostAddress address, quint32 port, QString mediaDir = "/media/", QString fileName = QString());
    ~Ingest();

    bool start();

private:
    QNetworkInterface m_interface;
    QHostAddress m_address;
    quint32 m_port;
    QString m_media_dir;
    QString m_file_name;

    QUdpSocket m_socket;
    QFile m_file;

    QReadWriteLock lock;
};

#endif // INGEST_H
