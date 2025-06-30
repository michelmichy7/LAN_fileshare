#include "Backend.h"
#include "QDebug"
#include <QCoreApplication>
#include <QUdpSocket>

Backend::Backend(QObject *parent)
    : QObject{parent}
{
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
    if (!m_model) {
       m_model = new ListModel(this);
    }

    if (!catcherSocket) {
        catcherSocket = new QUdpSocket(this);
        connect(catcherSocket, &QUdpSocket::readyRead, this, &Backend::onReadyRead);
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
        QHostAddress senderIP;
        QByteArray datagram;
        datagram.resize(catcherSocket->pendingDatagramSize());
        quint16 senderPort;



        catcherSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

       // QStringList devList = m_listModel->stringList();
        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device";
            catcherSocket->writeDatagram(QByteArray("DEVICE: "), QHostAddress::Broadcast, 45454);
            QString devName = senderIP.toString();
            m_model->addItem(devName);
        }
    }
}

void ListModel::handleDevClick(int index)
{

}


void ListModel::addItem(const QString &item)
{
    QStringList list = stringList();
    list.append(item);
    setStringList(list);
}
