#include "Backend/udpside.h"
#include "Backend/Backend.h"


UDPManager::UDPManager(Backend* backend, QObject *parent)
    : QObject(parent), m_backend(backend)
{
    senderSocket = new QUdpSocket(this);
    bool success = senderSocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
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
        if (m_backend->connectionState() == StatusClass::TCP_CONNECTED) {
            qDebug() << "UDP: Another device tried connection, declined - already connected to another";
            return;
        }

        QHostAddress senderIP;
        QByteArray datagram;
        quint16 senderPort;

        datagram.resize(senderSocket->pendingDatagramSize());

        qint64 bytesRead = senderSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);

        if (bytesRead == -1) {
            qDebug() << "Failed to read datagram:" << senderSocket->errorString();
            continue;
        }

        QString rawIP = senderIP.toString();

        // Handle IPv4-mapped IPv6
        if (rawIP.startsWith("::ffff:")) {
            rawIP = rawIP.mid(7);
        }

        // Ignore packets from self
        // Ignore packets from self (local IPs or localhost)
        if (m_localIPs.contains(rawIP) || rawIP == "127.0.0.1") {
            qDebug() << "Ignored self-response from:" << rawIP;
            continue;
        }


        qDebug() << "Received datagram from:" << rawIP << "Content:" << datagram;

        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device at:" << rawIP;
            m_backend->addDev_ToList(rawIP);
        }
        else if (datagram == "CONNECTION_REQUEST") {
            qDebug() << "::Received connection request from:" << rawIP;
            emit showConnectionPage(rawIP);
            qDebug("ConBox emitted");
        }
        else if (datagram == "CONNECTION_APPROVED") {
            qDebug() << "Connection approved by:" << rawIP;
            m_backend->setConnectionState(StatusClass::CONNECTION_APPROVED);
        }
        else if (datagram == "TCP_CONNECTED") {
            qDebug() << "Connected: " << rawIP;
            emit tcpConnected(rawIP);
        }
    }
}
