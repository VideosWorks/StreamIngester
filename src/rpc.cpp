#include "rpc.h"

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

Rpc::Rpc(quint32 port) : m_port(port)
{
    m_rpc_methods["addChannel"] = [this](QJsonArray params)
    {
        if (params.size() != 4)
            return false;

        QString channelUid = params.first().toString();
        QString streamType = params.at(1).toString();
        QHostAddress address(params.at(2).toString());
        quint32 port = params.at(3).toInt();

        emit addChannel(channelUid, streamType, address, port);

        return true;
    };
}

Rpc::~Rpc()
{

}



bool Rpc::listen()
{
    QTcpServer *server = new QTcpServer;
    bool ok = server->listen(QHostAddress::Any, m_port);

    if (!ok)
        return false;

    connect(server, &QTcpServer::newConnection, [this, server]()
    {
        qDebug() <<"== received connection from remote";
        QTcpSocket *client = server->nextPendingConnection();

        QObject::connect(client, &QTcpSocket::readyRead, [this, client]()
        {
            QJsonObject request = QJsonDocument::fromJson(client->readAll()).object();
            if (request.isEmpty())
            {
               qCritical() << "-- unable to parse command";
               return;
            }

            qDebug() << request;

            qreal version = request.value("jsonrpc").toVariant().toReal();
            qDebug() << "json rpc version recevied:" << version;
            if (version < 2.0)
            {
               qCritical() << "-- invalid json rpc version, ignoring";
               return;
            }

            QString method = request.value("method").toString();
            if (!m_rpc_methods.contains(method))
            {
               qWarning() << "== json rpc method not implemented!";
               return;
            }

            QString rpc_id = request.value("id").toString();

            QJsonArray params = request.value("params").toArray();

            qDebug() << "== response from rpc method:" << m_rpc_methods[method](params);

        });
    });

    return ok;
}
