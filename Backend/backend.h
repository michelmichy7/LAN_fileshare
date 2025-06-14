#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE void beSender();
    Q_INVOKABLE void beReceiver();

private:
    QUdpSocket *senderSocket = nullptr;
    QUdpSocket *receiverSocket;

    void findToSend();
    void findToReceive();

private slots:
    void onReadyRead();
};


#endif // BACKEND_H
