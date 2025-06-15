#include "Backend.h"
#include "QDebug"
#include <QCoreApplication>
#include <QUdpSocket>

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    catchPacket();
    sendPacket();

    connect(catcherSocket, &QUdpSocket::readyRead, this, &Backend::onReadyRead);
}

void Backend::sendPacket() {
    if (!senderSocket) {
        senderSocket = new QUdpSocket(this);
        qDebug() << "Sended packet for finding a device";

        senderSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        senderSocket->writeDatagram(QByteArray("FIND_DEVICE"), QHostAddress::Broadcast, 45454);

    }
}

void Backend::catchPacket() {
    if (!catcherSocket) {
        catcherSocket = new QUdpSocket(this);
        bool success = catcherSocket->bind(QHostAddress::Any, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        if (!success) {
            qDebug() << "Bind Failed: " << catcherSocket->errorString();
            return;
        }
    }
}

void Backend::onReadyRead()
{
    while (catcherSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(catcherSocket->pendingDatagramSize());
        quint16 senderPort;

        catcherSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device";
        }
    }

}

