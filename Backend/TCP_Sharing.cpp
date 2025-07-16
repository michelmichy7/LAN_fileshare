#include "Backend.h"

void Backend::tcpConnection_REC(const QString &ip) {
    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);

        tcpSocket->connectToHost(ip, 45454);

        if (tcpSocket->ConnectedState) {
            qDebug() << "Connected TCP socket: " << ip;
            emit tcpConnected(ip);
        }
    }


}

void Backend::tcpConnection_SEN(const QString &ip) {
    if (!tcpServer) {
        tcpServer = new QTcpServer(this);


    tcpServer->listen(QHostAddress(ip), 45454);

    if (tcpServer->hasPendingConnections()) {
        qDebug() << "pending: " << ip;
    }

    }
}


