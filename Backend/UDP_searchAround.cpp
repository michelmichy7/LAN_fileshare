#include "Backend.h"
#include "QDebug"
#include <QCoreApplication>
#include <QUdpSocket>
#include <QThread>

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    if (!m_model) {
        m_model = new ListModel(this);
        connect(m_model, &ListModel::doConnectionBox, this, &Backend::onDoConnectionBox);
    }
}

void Backend::sendPacket() {
    if (!senderSocket) {
        senderSocket = new QUdpSocket(this);
        connect(senderSocket, &QUdpSocket::readyRead, this, &Backend::onSReadyRead);

        senderSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        senderSocket->writeDatagram(QByteArray("FIND_DEVICE"), QHostAddress::Broadcast, 45454);
        qDebug() << "Sended packet to find Sender";
    }
}

void Backend::catchPacket() {
    if (!catcherSocket) {
        catcherSocket = new QUdpSocket(this);
        connect(catcherSocket, &QUdpSocket::readyRead, this, &Backend::onCReadyRead);
        bool success = catcherSocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

        if (!success) {
            qDebug() << "Bind Failed: " << catcherSocket->errorString();
            return;
        }
    }

}

void Backend::onCReadyRead()
{

    while (catcherSocket->hasPendingDatagrams()) {
        qDebug() << "Socket state:" << catcherSocket->state();
        qDebug() << "Socket bound to:" << catcherSocket->localPort();
        QHostAddress senderIP;
        QByteArray datagram;
        datagram.resize(catcherSocket->pendingDatagramSize());
        quint16 senderPort;

        catcherSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device";
            QString rawIP = senderIP.toString();

            if (rawIP.startsWith("::ffff:")) {
                rawIP = rawIP.mid(7);
            }
            m_model->addItem(rawIP);

            qDebug() << "Current model size:" << m_model->rowCount();
            QStringList allItems = m_model->stringList();


            for (const QString &item : std::as_const(allItems)) {
                qDebug() << item;
            }

        } else if (datagram == "CONNECT_REQUEST"){
            qDebug() << "Received connection request:" << senderIP;
            emit showConnectionPage();
        }
        }
    }

void Backend::onSReadyRead()
{

    while (senderSocket->hasPendingDatagrams()) {
        qDebug() << "Socket state:" << catcherSocket->state();
        qDebug() << "Socket bound to:" << catcherSocket->localPort();
        QHostAddress senderIP;
        QByteArray datagram;
        datagram.resize(senderSocket->pendingDatagramSize());
        quint16 senderPort;

        senderSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device";
            QString rawIP = senderIP.toString();

            if (rawIP.startsWith("::ffff:")) {
                rawIP = rawIP.mid(7);
            }
            m_model->addItem(rawIP);

            qDebug() << "Current model size:" << m_model->rowCount();
            QStringList allItems = m_model->stringList();


            for (const QString &item : std::as_const(allItems)) {
                qDebug() << item;
            }

        } else if (datagram == "CONNECT_REQUEST"){
            qDebug() << "Received connection request:" << senderIP;
            emit showConnectionPage();
        }
    }
}

void Backend::onDoConnectionBox(const QString &ip)
{
    if (!senderSocket) {
        senderSocket = new QUdpSocket(this);

        // Bind to any free port for sending only, before writing
        bool success = senderSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

        if (!success) {
            qDebug() << "Bind Failed: " << senderSocket->errorString();
            return;
        }
    }

    QByteArray data("CONNECT_REQUEST");
    senderSocket->writeDatagram(data, QHostAddress(ip), 45454);
}



void ListModel::handleDevClick(int index)
{
    QStringList list = stringList();
    QString ipAddr = list.at(index);

    qDebug() << "Gonna make an action with: " << ipAddr;
    emit doConnectionBox(ipAddr);
}

void ListModel::addItem(const QString &item)
{
    QStringList list = stringList();
    list.append(item);
    setStringList(list);
}

