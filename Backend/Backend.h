#ifndef BACKEND_H
#define BACKEND_H



#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QHostAddress>
#include <QStringListModel>

#include "Backend/tcpside.h"
#include "Backend/udpside.h"

class StatusClass
{
    Q_GADGET
public:
    //explicit StatusClass();

    enum ConnectionState {
        IDLE = 0,
        DISCOVERING_DEVICES,
        TOLD_ABOUT_SELF,
        REQUESTING_FOR_CONNECTION,
        CONNECTION_APPROVED,
        TCP_CONNECTED,
        TCP_DISCONNECTED
    };
    Q_ENUM(ConnectionState)
};



class ListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit ListModel(QObject *parent = nullptr)
        : QStringListModel(parent)
    {
        setStringList(QStringList());
    }

    Q_INVOKABLE void handleDevClick(int index);
    Q_INVOKABLE void addItem(const QString &item);

signals:
    void doConnectionBox(const QString &ip);

};



class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* model READ model CONSTANT)
    Q_PROPERTY(QString theirIp READ theirIp WRITE setTheirIp NOTIFY theirIPChanged)
    Q_PROPERTY(QObject* udpManager READ udpManager CONSTANT)
    Q_PROPERTY(QObject* tcpManager READ tcpManager CONSTANT)
    Q_PROPERTY(StatusClass::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)


public:
    explicit Backend(QObject *parent = nullptr);
    ListModel *m_model = nullptr;

    QObject* udpManager() const;
    QObject* tcpManager() const;
    Q_INVOKABLE void setConnectionState(StatusClass::ConnectionState state);

    ListModel* model() const { return m_model; }

    Q_INVOKABLE void tcpConnection_REC(const QString &ip);
    Q_INVOKABLE void tcpConnection_SEN(const QString &ip);
    QString message;
    Q_INVOKABLE void transferFilesTCP();

    void addDev_ToList(const QString &ip);

    QString theirIp() const { return m_theirValue.toString(); }

    void setTheirIp(QString &val) {
        QHostAddress addr(val);
        if (m_theirValue != addr) {
            m_theirValue = addr;
            emit theirIPChanged();
        }
    }

    StatusClass::ConnectionState connectionState() const {
        // You must return the actual state from somewhere — e.g.:
        return m_connectionState;
    }


signals:
    void showConnectionPage(const QString &message);
    void tcpConnected(const QString &ip);
    void udpSecHost(const QString &ip);
    void theirIPChanged();

    void connectionStateChanged();
private:
    //void sendStatusPacket(const QString &ip);
    UDPManager m_udpManager;
    TCPManager m_tcpManager;
    QHostAddress m_theirValue;

private:
    StatusClass::ConnectionState m_connectionState = StatusClass::IDLE;


private slots:
    //void onDoConnectionBox(const QString &ip);
};

class FileDialogHelper : public QObject
{
    Q_OBJECT
public:
    explicit FileDialogHelper(QObject *parent = nullptr);

    Q_INVOKABLE QStringList openFileDialog();
};

#endif // BACKEND_H
