#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QList>
#include <QPair>
#include <QHostAddress>
#include <QStringListModel>

class Backend : public QObject
{
    Q_OBJECT

//          --UDP Finder--
public:
    explicit Backend(QObject *parent = nullptr);
    void sendPacket();
    void catchPacket();


private:
    QStringListModel* m_listModel;
    QStringList currentList = m_listModel->stringList();
    QUdpSocket *senderSocket = nullptr;
    QUdpSocket *catcherSocket = nullptr;
    QHostAddress senderIP;
    QString senderIPadd = senderIP.toString();
private slots:
    void onReadyRead();


//      --Managing--
public:



private:
};

class ListModel : public QStringListModel
{
    Q_OBJECT
public:
    explicit ListModel (QObject *parent = nullptr)
        : QStringListModel(parent)
    {
        setStringList(QStringList());
    }
    Q_INVOKABLE void handleDevClick(int index);
    void changeDevList();
};

#endif // BACKEND_H
