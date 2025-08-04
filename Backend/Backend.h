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

//Structs

struct FileHeader {
    QString name;
    qint64 size;
};

class FileDialogHelper : public QObject
{
    Q_OBJECT
public:
    explicit FileDialogHelper(QObject *parent = nullptr);
    Q_PROPERTY(QStringList selectedFiles READ selectedFiles NOTIFY selectedFilesChanged)
    QStringList m_selectedFiles;
    Q_INVOKABLE QStringList openFileDialog();
    QStringList selectedFiles() const { return m_selectedFiles; }
signals:
    void selectedFilesChanged();
};

class StatusClass
{
    Q_GADGET
public:
    //explicit StatusClass();

    enum ConnectionState {
        IDLE = 0,
        DISCOVERING_DEVICES, //if going back to this state, system wouldnt have to send packet everytime, to prevent spam of packets
        TOLD_ABOUT_SELF,
        REQUESTING_FOR_CONNECTION,
        CONNECTION_APPROVED,

        TCP_CONNECTED,
        TCP_DISCONNECTED
    };
    enum ActivityState {
        NONE = 0,
        SELECTING_FILES,
        SENDING_FILES,
        RECEIVING_FILES
    };

    Q_ENUM(ConnectionState)
    Q_ENUM(ActivityState)
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

    Q_PROPERTY(FileDialogHelper* filesManager READ getFilesManager CONSTANT)

    //                    ----- States -----
    Q_PROPERTY(StatusClass::ConnectionState connectionState READ connectionState NOTIFY connectionStateChanged)
    Q_PROPERTY(StatusClass::ActivityState activityState READ activityState NOTIFY activityStateChanged)

public:
    StatusClass::ActivityState activityState() const {
        return m_activityState;
    }
    StatusClass::ConnectionState connectionState() const {
        return m_connectionState;
    }
    Q_INVOKABLE void setConnectionState(StatusClass::ConnectionState state);
    Q_INVOKABLE void setActivityState(StatusClass::ActivityState state);

private:
    StatusClass::ConnectionState m_connectionState = StatusClass::IDLE;
    StatusClass::ActivityState m_activityState = StatusClass::NONE;
signals:
    void connectionStateChanged();
    void activityStateChanged();


private:


private:

    FileDialogHelper* getFilesManager() { return &m_filesManager; }

public:
    explicit Backend(QObject *parent = nullptr);
    FileHeader fh;

    ListModel *m_model = nullptr;
    FileDialogHelper m_filesManager;

    QObject* udpManager() const;
    QObject* tcpManager() const;
    QObject* filesManager() const;


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

signals:
    void showConnectionPage(const QString &message);
    void tcpConnected(const QString &ip);
    void udpSecHost(const QString &ip);
    void theirIPChanged();


private:
    //void sendStatusPacket(const QString &ip);
    UDPManager m_udpManager;
    TCPManager m_tcpManager;

    QHostAddress m_theirValue;




private slots:
    //void onDoConnectionBox(const QString &ip);
};




#endif // BACKEND_H
