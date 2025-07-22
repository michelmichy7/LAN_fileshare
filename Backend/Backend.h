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


public:
    ListModel *m_model = nullptr;
    explicit Backend(QObject *parent = nullptr);
    ListModel* model() const { return m_model; }

    Q_INVOKABLE void sendPacket();
    Q_INVOKABLE void catchPacket();

    Q_INVOKABLE void tcpConnection_REC(const QString &ip);
    Q_INVOKABLE void tcpConnection_SEN(const QString &ip);
    QString message;
    Q_INVOKABLE void transferFilesTCP();

signals:
    void showConnectionPage(const QString &message);
    void tcpConnected(const QString &ip);

private:
    QUdpSocket *senderSocket = nullptr;
    QUdpSocket *catcherSocket = nullptr;


    //void sendStatusPacket(const QString &ip);


private slots:
    //void onReadyRead();
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
