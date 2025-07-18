#ifndef UDPSIDE_H
#define UDPSIDE_H

#include "Backend/Backend.h"
#include <QObject>
#include <QUdpSocket>
#include <QDebug>

class UDPSender : public QObject
{
    Q_OBJECT
public:
    explicit UDPSender(QObject *parent = nullptr);

    Q_INVOKABLE void sendPacket();
signals:

private:
    QUdpSocket *senderSocket = nullptr;


private slots:
    void onDoConnectionBox(const QString &ip);
    void onReadyRead();
};




class UDPReceiver : public QObject
{
    Q_OBJECT
public:
    explicit UDPReceiver(QObject *parent = nullptr);

    Q_INVOKABLE void catchPacket();
    Q_INVOKABLE void sendPacket();
signals:
    void showConnectionPage(const QString &message);
private slots:
    void onReadyRead();

private:
    Backend backend;
    QUdpSocket *catcherSocket = nullptr;
};

#endif // UDPSIDE_H
