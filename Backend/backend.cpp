#include "backend.h"
#include "QDebug"
#include <QCoreApplication>
#include <QUdpSocket>
#include <QTimer>

Backend::Backend(QObject *parent)
    : QObject{parent}
{}

void Backend::find_Lan_Devices() {
    qDebug() << "Code runs!";
    if (!senderSocket) {
        senderSocket = new QUdpSocket(this);
    }

    QByteArray message = "DISCOVER_DEVICES";
    QHostAddress broadcastAddress = QHostAddress::Broadcast;
    quint16 port = 45454;

    //QTimer::singleShot(1000, [&]() {
        senderSocket->writeDatagram(message, broadcastAddress, port);
        qDebug() << "Broadcast message sent";
    //});


}
