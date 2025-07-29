#include "Backend.h"

#include "tcpside.h"

#include <QFile>

TCPManager::TCPManager(Backend* backend, QObject *parent)
    : QObject(parent)
{
    tcpServer = nullptr;
    tcpSocket = nullptr;
}

void TCPManager::startServer(quint16 port)
{

}

void TCPManager::connectToHost(const QString &ip, quint16 port)
{

}

void TCPManager::sendData(const QByteArray &data)
{

}

void TCPManager::onNewConnection()
{

}

void TCPManager::onReadyRead()
{

}

void TCPManager::onDisconnected()
{

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

