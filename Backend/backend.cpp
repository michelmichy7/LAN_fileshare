#include "backend.h"
#include "QDebug"
#include <QCoreApplication>
#include <QUdpSocket>

Backend::Backend(QObject *parent)
    : QObject{parent}
{}

void Backend::onReadyRead() {
    while (receiverSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress sender;
        quint16 senderPort;

        datagram.resize(receiverSocket->pendingDatagramSize());
        receiverSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        qDebug() << "Received: " << datagram << "from" << sender.toString() << senderPort;

        if (datagram == "DISCOVER_DEVICES") {
            QByteArray response = "DISCOVER_RESPONSE";
            receiverSocket->writeDatagram(response, sender, senderPort);
            qDebug() << "Sent response to" << sender.toString();
        }
    }
}

void Backend::findToSend() {
    qDebug() << "Code runs!";
    if (!senderSocket) {
        senderSocket = new QUdpSocket(this);
        senderSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    }

    QByteArray message = "DISCOVER_DEVICES";
    QHostAddress broadcastAddress = QHostAddress::Broadcast;
    quint16 port = 45454;


    senderSocket->writeDatagram(message, broadcastAddress, port);
}

void Backend::findToReceive() {
    receiverSocket = new QUdpSocket(this);

    bool success = receiverSocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    if (!success) {
        qDebug() << "Bind failed:" << receiverSocket->errorString();
        return;
    }

    connect(receiverSocket, &QUdpSocket::readyRead, this, &Backend::onReadyRead);

    qDebug() << "UDP Receiver is running on port 45454";

}


void Backend::beSender()
{
    findToSend();
}

void Backend::beReceiver()
{
    findToReceive();
}


