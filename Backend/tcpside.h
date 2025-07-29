#ifndef TCPSIDE_H
#define TCPSIDE_H

#include "Backend/Backend.h"

#include <QObject>
#include <QTcpServer>



class TCPManager : public QObject
{
    Q_OBJECT

public:
    explicit TCPManager(Backend* backend, QObject *parent = nullptr);

private:
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    Backend *m_backend = nullptr;
};


/*
class TCPServer : public QObject
{
    Q_OBJECT

public:
    explicit TCPServer(QObject *parent = nullptr);
    Q_INVOKABLE void tcpConnection_SEN(const QString &ip);

private:
    QTcpServer *tcpServer = nullptr;

signals:
    void tcpConnected(const QString &ip);
};

class TCPClient : public QObject
{
    Q_OBJECT

public:
    explicit TCPClient(Backend *m_backend, QObject *parent = nullptr);
    Q_INVOKABLE void transferFilesTCP();
    Q_INVOKABLE void tcpConnection_REC(const QString &ip);

private:
    QTcpSocket *tcpSocket = nullptr;
    Backend *m_backend = nullptr;
};
*/
#endif // TCPSIDE_H
