#ifndef TCPSIDE_H
#define TCPSIDE_H


#include <QObject>
#include <QTcpServer>


class Backend;
class ListModel;

class TCPManager : public QObject
{
    Q_OBJECT

public:
    explicit TCPManager(Backend* backend, QObject *parent = nullptr);

    void startServer(quint16 port = 45454);
    void connectToHost(const QHostAddress &ip, quint16 port);
    void extracted();
    void sendData();
    void doTCP_Connection();

private:
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *tcpSocket = nullptr;
    Backend *m_backend = nullptr;

    QList<QTcpSocket*> m_clientSockets; // For storing connected clients on server


private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

signals:
    void dataReceived(const QByteArray &data);
    void clientConnected(const QString &ip);
    void clientDisconnected();
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
