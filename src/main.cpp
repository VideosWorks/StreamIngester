#include "rpc.h"
#include "ingest.h"

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkInterface>


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2)
    {
        qDebug() << "usage: streamingester [rpc_port=1338] [media_dir=/media] [interface=ANY]";
    }

    bool ok = false;
    quint32 port = QString(argv[1]).toLong(&ok);
    if (!ok || port <= 0)
    {
        port = 1338;
    }

    QString mediaDir(argc > 2 ? argv[2] : "/media");
    QNetworkInterface interface = argc > 3 ? QNetworkInterface::interfaceFromName(argv[3]) : QNetworkInterface();

    Rpc rpc(port);

    if (!rpc.listen())
    {
        qCritical() << "error while trying to listen on port:" << port;
        return 1;
    }
    qDebug() << "listening for rpc messages on port:" << port;
    qDebug() << "media directory:" << mediaDir;
    qDebug() << "listening on interface:" << (interface.isValid() ? interface.humanReadableName() : "ANY");

    QObject::connect(&rpc, &Rpc::addChannel, [&interface, mediaDir](QString channelUid, QString type, QHostAddress address, qint32 port)
    {
        qDebug() << "add new channel:" <<  QString("%1 %2 %3 %4").arg(channelUid).arg(type).arg(address.toString()).arg(QString::number(port));

        auto channel = new Ingest(interface, address, port, mediaDir);
        if (!channel->start())
            qDebug() << "ingesting failed";

    });

    return a.exec();
}
