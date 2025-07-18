#include "udpside.h"

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
