#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QHostAddress>

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
    QString senderIPadd = senderIP.toString();
private slots:
    void onReadyRead();


//      --Managing--
public:
    QList<QPair<QString, QString>> computers;

    Q_INVOKABLE int getPcsCount() const {
        return computers.size();
    }

    Q_INVOKABLE QString getPcName(int index) const {
        if (index >= 0 && index < computers.size()) {
            return computers[index].first;
        }
        return QString();
    }
    Q_INVOKABLE QString getPcIp(int index) const {
        if (index >= 0 && index < computers.size()) {
            return computers[index].second;
        }
        return QString();
    }
private:

};


#endif // BACKEND_H
