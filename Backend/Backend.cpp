#include "Backend.h"
#include <QTimer>

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_udpManager(this), m_tcpManager(this)
{
    m_model = new ListModel(this);
    //Q_PROPERTY(StatusClass::State connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(StatusClass::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(StatusClass::ActivityState activityState READ activityState NOTIFY activityStateChanged)
}

QObject* Backend::udpManager() const {
    return (QObject*)&m_udpManager;
}

QObject* Backend::tcpManager() const {
    return (QObject*)&m_tcpManager;
}

void Backend::setConnectionState(StatusClass::ConnectionState state)
{
    if (m_connectionState == state)
        return;

    m_connectionState = state;

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
        qDebug() << "STORED:" << m_theirValue;
        m_udpManager.sendPacket("CONNECTION_APPROVED", m_theirValue);
        m_tcpManager.startServer();
        m_tcpManager.connectToHost(m_theirValue, 45454);
    }
    else if (state == StatusClass::TCP_CONNECTED) {
        //m_udpManager.sendPacket("TCP_REQUEST", m_theirValue);
    }
        emit connectionStateChanged();
}

void Backend::setActivityState(StatusClass::ActivityState state) {
    if (m_activityState == state)
        return;
    m_activityState = state;

     if (state == StatusClass::SENDING_FILES) {
        m_tcpManager.sendData();
    } else if (state == StatusClass::RECEIVING_FILES) {
         m_filesManager.openFileDialog();
    } else if (state == StatusClass::SELECTING_FILES) {
        m_filesManager.openFileDialog();
    }
    emit activityStateChanged();
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
    if (!list.contains(item)) {
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



