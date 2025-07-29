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
    if (state == StatusClass::DISCOVERING_DEVICES) {
        //for loop in next
        qDebug() << "Connection state set to: " << state;
        m_udpManager.sendPacket("FIND_DEVICE");
    }
    else if (state == StatusClass::TOLD_ABOUT_SELF) {
        qDebug() << "Connection state set to: " << state;
        m_udpManager.sendPacket("FIND_DEVICE");
    }
    else if (state == StatusClass::REQUESTING_FOR_CONNECTION) {
        m_udpManager.sendPacket("CONNECTION_REQUEST", m_theirValue);
    }
    else if (state == StatusClass::CONNECTION_APPROVED) {
        m_udpManager.sendPacket("CONNECTION_APPROVED", m_theirValue);
    }
    else if (state == StatusClass::TCP_CONNECTED) {
        m_udpManager.sendPacket("TCP_REQUEST", m_theirValue);
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


//these two were added because of definition error
void ListModel::addItem(const QString &item)
{
    QStringList list = stringList();
    if (!list.contains(item)) { // optional: prevent duplicates
        list.append(item);
        setStringList(list);
    }
}


void ListModel::handleDevClick(int index)
{
    const QStringList list = stringList();
    if (index >= 0 && index < list.size()) {
        emit doConnectionBox(list.at(index));
    }
}



