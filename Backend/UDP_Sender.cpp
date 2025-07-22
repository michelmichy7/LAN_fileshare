#include "udpside.h"

UDPSender::UDPSender(QObject *parent)
    : QObject{parent}
{
    senderSocket = new QUdpSocket(this);
    bool success = senderSocket->bind(QHostAddress::AnyIPv4, 0,
                                      QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!success) {
        qDebug() << "UDP bind failed:" << senderSocket->errorString();
    }
}

void UDPSender::catchPacket()
{
    if (!senderSocket) {

        senderSocket = new QUdpSocket(this);
        connect(senderSocket, &QUdpSocket::readyRead, this, &UDPSender::onReadyRead);

        bool success = senderSocket->bind(QHostAddress::Any, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        if (!success) {
            qDebug() << "Bind Failed: " << senderSocket->errorString();
            return;
        }
    }
}

void UDPSender::sendStatusPacket(const QString& ip)
{
    QByteArray data = "TCP_CONNECTED";
    senderSocket->writeDatagram(data, QHostAddress(ip), 45454);
    qDebug() << "UDP status packet sent to" << ip;
}

void UDPSender::onDoConnectionBox(const QString &ip)
{
    if (!senderSocket) {
        senderSocket = new QUdpSocket(this);

        // Bind to any free port for sending only, before writing
        bool success = senderSocket->bind(QHostAddress::AnyIPv4, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
        qDebug() << "onConnection";
        if (!success) {
            qDebug() << "Bind Failed: " << senderSocket->errorString();
            return;
        }
    }

    QByteArray data("CONNECT_REQUEST");
    senderSocket->writeDatagram(data, QHostAddress(ip), 45454);
}

void UDPSender::sendPacket() {
    //change this
    senderSocket->bind(QHostAddress::Any, 0, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    senderSocket->writeDatagram(QByteArray("FIND_DEVICE"), QHostAddress::Broadcast, 45454);
    qDebug() << "Sended packet to find Sender";
}

void UDPSender::onReadyRead()
{
    while (senderSocket->hasPendingDatagrams()) {
        QHostAddress senderIP;
        QByteArray datagram;
        quint16 senderPort;

        // Resize the datagram buffer BEFORE reading
        datagram.resize(senderSocket->pendingDatagramSize());

        // Read the datagram
        qint64 bytesRead = senderSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

        if (bytesRead == -1) {
            qDebug() << "Failed to read datagram:" << senderSocket->errorString();
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
