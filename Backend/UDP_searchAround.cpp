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

        //std::string devName = senderIP.toString();

        catcherSocket->readDatagram(datagram.data(), datagram.size(), &senderIP, &senderPort);


        if (datagram == "FIND_DEVICE") {
            qDebug() << "Found a Device";
            // QStringList devList = m_listModel->stringList();
            m_model->addItem("s"); //problem with showing in the list, it will add to the list tho
            qDebug() << "Current model size:" << m_model->rowCount();
            // Force UI update (if needed)
            emit m_model->dataChanged(m_model->index(0), m_model->index(m_model->rowCount()-1));
        } else {
            qDebug() << "Received unknown packet:" << datagram;
        }
        }
    }


void ListModel::handleDevClick(int index)
{

}


void ListModel::addItem(const QString &item)
{
    QStringList list = stringList();
    list.append(item); //problem with showing in the list
    setStringList(list);
    // Emit signals to notify QML
    emit dataChanged(index(0), index(rowCount()-1));
    emit layoutChanged();
}
