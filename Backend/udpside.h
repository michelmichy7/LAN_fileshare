#ifndef UDPSIDE_H
#define UDPSIDE_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QNetworkInterface>


class Backend;
class ListModel;

class UDPManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* model READ model CONSTANT)

public:
    explicit UDPManager(Backend* backend, QObject *parent = nullptr);
    ListModel* model() const { return m_model; }
    Q_INVOKABLE void catchPacket();
    Q_INVOKABLE void sendPacket(const QString &datagram, QHostAddress ip = QHostAddress::Broadcast);

    QString ipOF_HOST;

signals:
    void showConnectionPage(const QString &message);
    void tcpConnected(const QString &ip);

private:
    QUdpSocket *senderSocket = nullptr;

    Backend* m_backend = nullptr;
    ListModel* m_model = nullptr;

    QStringList getLocalIPs();
    QStringList m_localIPs;

public slots:
    void sendStatusPacket(const QString& ip);


private slots:
    void onDoConnectionBox(const QString &ip);

    void onReadyRead();
};


/*
class UDPReceiver : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* model READ model CONSTANT)
public:
    explicit UDPReceiver(Backend* backend, QObject *parent = nullptr);
    ListModel* model() const { return m_model; }

    Q_INVOKABLE void catchPacket();
    Q_INVOKABLE void sendPacket();


signals:
    void showConnectionPage(const QString &message);

    void tcpConnected(const QString &ip);


private slots:
    void onReadyRead();
    void onDoConnectionBox(const QString &ip);

private:
    Backend* m_backend;
    ListModel* m_model = nullptr;
    QUdpSocket *catcherSocket = nullptr;
};*/

#endif // UDPSIDE_H
