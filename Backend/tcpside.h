#ifndef TCPSIDE_H
#define TCPSIDE_H

#include <QObject>
#include <QTcpServer>

class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = nullptr);

private:
    QTcpServer *tcpServer = nullptr;

signals:
};

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = nullptr);

private:
    QTcpSocket *tcpSocket = nullptr;

};

#endif // TCPSIDE_H
