#include "Backend.h"

#include "tcpside.h"

#include <QFile>

TCPManager::TCPManager(Backend* backend, QObject *parent)
    : QObject(parent)
{
    tcpServer = nullptr;
    tcpSocket = nullptr;
}

void TCPManager::doTCP_Connection() {

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
    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);

        connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);
        connect(tcpSocket, &QTcpSocket::connected, this, []() {
            qDebug() << "Connected to server!";
        });
    }

    tcpSocket->connectToHost(ip, port);
}


void TCPManager::sendData(const QByteArray &data)
{
    if (tcpSocket && tcpSocket->state() == QTcpSocket::ConnectedState) {
        tcpSocket->write(data);
    } else {
        qDebug() << "Not connected to any host.";
    }
}


void TCPManager::onNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);

    // Store the client socket if you want to communicate with it later
    m_clientSockets.append(clientSocket);

    qDebug() << "New client connected from" << clientSocket->peerAddress().toString();
}

void TCPManager::onReadyRead()
{
        QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
        if (!socket) return;

        QByteArray data = socket->readAll();
        qDebug() << "Received data:" << data;



}

void TCPManager::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "Client disconnected:" << socket->peerAddress().toString();

    m_clientSockets.removeAll(socket);
    socket->deleteLater();
}


/*
 TCPSide::TCPSide(QObject *parent)
    : QObject{parent}
{}

TCPClient::TCPClient(Backend *m_backend, QObject *parent)
    : QObject{parent}, m_backend(m_backend)
{

}

void TCPClient::tcpConnection_REC(const QString &ip) {
    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);

        tcpSocket->connectToHost(ip, 45454);

         connect(tcpSocket, &QTcpSocket::connected, this, [this, ip]()  {
            qDebug() << "Connected TCP socket: " << ip;
            emit m_backend->tcpConnected(ip);
        });
    }
}

TCPServer::TCPServer(QObject *parent)
    : QObject{parent}
{

}
void TCPServer::tcpConnection_SEN(const QString &ip) {
    if (!tcpServer) {
        tcpServer = new QTcpServer(this);

        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        if (clientSocket) {
            qDebug() << "Accepted TCP connection from: " << clientSocket->peerAddress().toString();
            emit tcpConnected(ip);
        }

    tcpServer->listen(QHostAddress(ip), 45454);
    }
}

    void TCPClient::transferFilesTCP() {
        FileDialogHelper dialog;
        QStringList pastedFiles = dialog.openFileDialog();
        if (!tcpSocket || tcpSocket->state() != QAbstractSocket::ConnectedState) {
            qDebug() << "TCP socket not connected.";
            return;
        }

        for (const QString &item: pastedFiles) {
            qDebug() << item;
        }

       /*
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open file:" << file.errorString();
            return;
        }

        QByteArray fileData = file.readAll();
        file.close();

        // Send file size (8 bytes)
        qint64 fileSize = fileData.size();
        tcpSocket->write(reinterpret_cast<char*>(&fileSize), sizeof(qint64));

        // Send file data
        tcpSocket->write(fileData);
        tcpSocket->flush();

        qDebug() << "Sent file over TCP: " << filePath;

    }
*/

