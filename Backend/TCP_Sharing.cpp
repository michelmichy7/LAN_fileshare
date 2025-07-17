#include "Backend.h"

void Backend::tcpConnection_REC(const QString &ip) {
    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);

        tcpSocket->connectToHost(ip, 45454);

        if (tcpSocket->ConnectedState) {
            qDebug() << "Connected TCP socket: " << ip;
            emit tcpConnected(ip);
            sendStatusPacket(ip);
        }
    }


}

void Backend::tcpConnection_SEN(const QString &ip) {
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

void Backend::sendFiles() {

}

