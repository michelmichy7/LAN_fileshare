#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    void sendPacket();
    void catchPacket();


private:

    //void findToSend();
    //void findToReceive();
    QUdpSocket *senderSocket = nullptr;
    QUdpSocket *catcherSocket = nullptr;
    QHostAddress senderIP;
private slots:
    void onReadyRead();
};


#endif // BACKEND_H
