#ifndef UDPSIDE_H
#define UDPSIDE_H

#include "Backend/Backend.h"
#include <QObject>
#include <QUdpSocket>
#include <QDebug>

class UDPSender : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* model READ model CONSTANT)

public:
    explicit UDPSender(Backend* backend, QObject *parent = nullptr);
    ListModel* model() const { return m_model; }
    Q_INVOKABLE void catchPacket();
    Q_INVOKABLE void sendPacket();

signals:
    void showConnectionPage(const QString &message);
    void tcpConnected(const QString &ip);

private:
    QUdpSocket *senderSocket = nullptr;

    Backend* m_backend = nullptr;
    ListModel* m_model = nullptr;

public slots:
    void sendStatusPacket(const QString& ip);


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

    ListModel* model() const { return m_model; }

signals:
    void showConnectionPage(const QString &message);

    void tcpConnected(const QString &ip);


private slots:
    void onReadyRead();
    void onDoConnectionBox(const QString &ip);

private:
    Backend backend;
    ListModel* m_model = backend.m_model;
    QUdpSocket *catcherSocket = nullptr;
};

#endif // UDPSIDE_H
