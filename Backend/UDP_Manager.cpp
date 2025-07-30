#include "Backend/udpside.h"
#include "Backend/Backend.h"


UDPManager::UDPManager(Backend* backend, QObject *parent)
    : QObject(parent), m_backend(backend)
{
    senderSocket = new QUdpSocket(this);
    bool success = senderSocket->bind(QHostAddress::Any, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    if (!success) {
        qDebug() << "Bind Failed: " << senderSocket->errorString();
        return;
    }
    m_model = backend->model();

    m_localIPs = getLocalIPs();
    m_backend->setConnectionState(StatusClass::DISCOVERING_DEVICES);

    qDebug() << "UDPManager created at" << this;

    connect(senderSocket, &QUdpSocket::readyRead, this, &UDPManager::onReadyRead);
}

void UDPManager::sendPacket(const QString &datagram, QHostAddress ip) {

    QByteArray data = datagram.toUtf8();
    qint64 bytesSent = senderSocket->writeDatagram(data,
                                                   ip,
                                                   45454);
    if (bytesSent == -1 && datagram == "FIND_DEVICE") {
        qDebug() << "Failed to send packet:" << senderSocket->errorString();
    } else {
        qDebug() << "Sended packet";
    }
}

QStringList UDPManager::getLocalIPs()
{
    QStringList result;
    for (const QHostAddress &addr : QNetworkInterface::allAddresses()) {
        if (addr.protocol() == QAbstractSocket::IPv4Protocol && !addr.isLoopback())
            result << addr.toString();
    }
    return result;
}

void UDPManager::catchPacket()
{

}
void UDPManager::sendStatusPacket(const QString& ip)
{
    QByteArray data = "TCP_CONNECTED";
    senderSocket->writeDatagram(data, QHostAddress(ip), 45454);
    qDebug() << "UDP status packet sent to" << ip;
}

void UDPManager::onDoConnectionBox(const QString &ip)
{
    qDebug() << "onConnection";
    QByteArray data("CONNECT_REQUEST");
    senderSocket->writeDatagram(data, QHostAddress(ip), 45454);
}

void UDPManager::onReadyRead()
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
            //if (!m_localIPs.contains(rawIP)) {
                m_backend->addDev_ToList(rawIP);
            //}
        }
        else if (datagram == "CONNECTION_REQUEST") {
            qDebug() << "::Received connection request from:" << rawIP;
            emit showConnectionPage(rawIP);
            qDebug("ConBox emitted");
        }
        else if (datagram == "CONNECTION_APPROVED") {

        }
        else if (datagram == "TCP_CONNECTED") {
            qDebug() << "Connected: " << rawIP;
            emit tcpConnected(rawIP);
        }
    }
}
