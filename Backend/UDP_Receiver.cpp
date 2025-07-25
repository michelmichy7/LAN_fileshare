#include "Backend.h"
#include "udpside.h"
#include "QDebug"
#include <QCoreApplication>
#include <QUdpSocket>
#include <QThread>

UDPReceiver::UDPReceiver(QObject *parent)
    : QObject{parent}
{
    connect(m_model, &ListModel::doConnectionBox, this, &UDPReceiver::onDoConnectionBox);
}

void UDPReceiver::catchPacket() {
    if (!catcherSocket) {

        catcherSocket = new QUdpSocket(this);
        connect(catcherSocket, &QUdpSocket::readyRead, this, &UDPReceiver::onReadyRead);

        bool success = catcherSocket->bind(QHostAddress::Any, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        if (!success) {
            qDebug() << "Bind Failed: " << catcherSocket->errorString();
            return;
        }
    }
}

void UDPReceiver::sendPacket()
{
    bool success = catcherSocket->bind(QHostAddress::Any, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!success) {
        qDebug() << "Bind Failed: " << catcherSocket->errorString();
        return;
    }
    //change this
    catcherSocket->writeDatagram(QByteArray("FIND_DEVICE"), QHostAddress::Broadcast, 45454);
    qDebug() << "Sended packet to find Sender";
}

void UDPReceiver::onReadyRead()
{
    while (catcherSocket->hasPendingDatagrams()) {
        QHostAddress senderIP;
        QByteArray datagram;
        quint16 senderPort;

        // Resize the datagram buffer BEFORE reading
        datagram.resize(catcherSocket->pendingDatagramSize());

        // Read the datagram
        qint64 bytesRead = catcherSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

        if (bytesRead == -1) {
            qDebug() << "Failed to read datagram:" << catcherSocket->errorString();
            continue;
        }

        // Convert IP address to string AFTER reading
        QString rawIP = senderIP.toString();

        // Handle IPv4-mapped IPv6 addresses
        if (rawIP.startsWith("::ffff:")) {
            rawIP = rawIP.mid(7);
        }

        qDebug() << "Received datagram from:" << rawIP << "Content:" << datagram;

        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device at:" << rawIP;
            m_model->addItem(rawIP);
            qDebug() << "Current model size:" << m_model->rowCount();

            QStringList allItems = m_model->stringList();
            for (const QString &item : std::as_const(allItems)) {
                qDebug() << item;
            }
        } else if (datagram == "CONNECT_REQUEST") {
            qDebug() << "Received connection request from:" << rawIP;
            emit showConnectionPage(rawIP);
        } else if (datagram == "TCP_CONNECTED") {
            qDebug() << "Connected: " << rawIP;
            emit tcpConnected(rawIP);
        }
    }
}

void UDPReceiver::onDoConnectionBox(const QString &ip)
{
    if (!catcherSocket) {
        catcherSocket = new QUdpSocket(this);

        // Bind to any free port for sending only, before writing
        bool success = catcherSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        qDebug() << "onConnection";
        if (!success) {
            qDebug() << "Bind Failed: " << catcherSocket->errorString();
            return;
        }
    }

    QByteArray data("CONNECT_REQUEST");
    catcherSocket->writeDatagram(data, QHostAddress(ip), 45454);
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
