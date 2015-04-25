#ifndef RPC_H
#define RPC_H

#include <QHash>
#include <QJsonArray>
#include <QHostAddress>
#include <functional>

class Rpc : public QObject
{
    Q_OBJECT

public:
    Rpc(quint32 port);
    ~Rpc();
    bool listen();

private:
    quint32 m_port;
    QHash<QString, std::function<int(QJsonArray params)>> m_rpc_methods;

signals:
    void addChannel(QString channelUid, QString type, QHostAddress address, quint32 port);
};

#endif // RPC_H
