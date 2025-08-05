#ifndef TCPSIDE_H
#define TCPSIDE_H


#include <QObject>
#include <QTcpServer>
#include <QMimeDatabase>
#include <QIODevice>


class Backend;
class ListModel;

class TCPManager : public QObject
{
    Q_OBJECT

public:
    explicit TCPManager(Backend* backend, QObject *parent = nullptr);

    void startServer(quint16 port = 45454);
    void connectToHost(const QHostAddress &ip, quint16 port);
    void sendData();
    void sendFile(const QString &filePath);
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


#endif // TCPSIDE_H
