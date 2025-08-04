#include "Backend.h"

#include "tcpside.h"

#include <QFile>
#include <qfileinfo.h>

TCPManager::TCPManager(Backend* backend, QObject *parent)
    : QObject(parent), m_backend(backend)
{
    tcpServer = nullptr;
    tcpSocket = nullptr;
}

void TCPManager::startServer(quint16 port)
{
    tcpServer = new QTcpServer(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &::TCPManager::onNewConnection);

    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start! Error:" << tcpServer->errorString();
        return;
    }

    qDebug() << "Server started on port" << port;
}

void TCPManager::connectToHost(const QHostAddress &ip, quint16 port = 45454)
{
    if (m_backend->connectionState() == StatusClass::TCP_CONNECTED) {
        qDebug() << "TCP: Another device tried connection, declined - already connected to another";
        return;
    }

    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);

        connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);
        connect(tcpSocket, &QTcpSocket::connected, this, [this]() {
            qDebug() << "Connected to server!";
            m_backend->setConnectionState(StatusClass::TCP_CONNECTED);
        });
    }

    tcpSocket->connectToHost(ip, port);
}


void TCPManager::sendData(const QString &filePath)
{
    qDebug("Sending data");
    for (int i = 0; i < m_backend->m_filesManager.m_selectedFiles.count(); ++i) {
        qDebug() << "Index:" << i << "File:" << m_backend->m_filesManager.m_selectedFiles[i];
    }
    if (tcpSocket && tcpSocket->state() == QTcpSocket::ConnectedState) {
        //tcpSocket->write(data);
    } else {
        qDebug() << "Not connected to any host.";
    }
}



void TCPManager::onNewConnection()
{
    if (!m_clientSockets.isEmpty()) {
        // Already have a client, reject the new connection immediately
        QTcpSocket *newSocket = tcpServer->nextPendingConnection();
        qDebug() << "Rejected new client from" << newSocket->peerAddress().toString() << "because a client is already connected";
        newSocket->disconnectFromHost();
        newSocket->deleteLater();
        return;
    }

    // No clients connected — accept this new connection
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);

    // Store the client socket if you want to communicate with it later
    m_clientSockets.append(clientSocket);

    qDebug() << "New client connected from" << clientSocket->peerAddress().toString();
}

void TCPManager::onReadyRead()
{
    qDebug() << "Data received (TCP stub).";
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    qDebug() << "Received data:" << data;



}

void TCPManager::onDisconnected()
{
    m_backend->setConnectionState(StatusClass::ConnectionState::TCP_DISCONNECTED);
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "Client disconnected:" << socket->peerAddress().toString();

    m_clientSockets.removeAll(socket);
    socket->deleteLater();
}
