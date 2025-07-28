#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_udpManager(this)
{
    m_model = new ListModel(this);
}

QObject* Backend::udpManager() const {
    return (QObject*)&m_udpManager;
}


void Backend::setConnectionState(StatusClass::ConnectionState state)
{
    if (state == 1) {
        qDebug() << "Connection state set to: " << state;
        m_udpManager.sendPacket("FIND_DEVICE");
    }
    else if (state == 2) {
        qDebug() << "Connection state set to: " << state;
        m_udpManager.sendPacket("FIND_DEVICE");
    }
    else if (state == 3) {
        m_udpManager.sendPacket("CONNECTION_REQUEST", m_theirValue);
    }

}


void Backend::tcpConnection_REC(const QString &ip)
{

}

void Backend::tcpConnection_SEN(const QString &ip)
{

}

void Backend::transferFilesTCP()
{

}

void Backend::addDev_ToList(const QString &ip)
{
    m_model->addItem(ip);
    qDebug() << "added: " << ip;

    QStringList allItems = m_model->stringList();
    for (const QString &item : std::as_const(allItems)) {
        qDebug() << item;
    }
}


