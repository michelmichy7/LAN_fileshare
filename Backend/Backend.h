#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QList>

class Backend : public QObject
{
    Q_OBJECT

//          --UDP Finder--
public:
    explicit Backend(QObject *parent = nullptr);
    void sendPacket();
    void catchPacket();


private:
    QUdpSocket *senderSocket = nullptr;
    QUdpSocket *catcherSocket = nullptr;
    QHostAddress senderIP;
private slots:
    void onReadyRead();


//      --Managing--
private:
    QList<QString>computer_items;
};


#endif // BACKEND_H
