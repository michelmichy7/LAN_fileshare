
#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>
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
};

class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* model READ model CONSTANT)

public:
    explicit Backend(QObject *parent = nullptr);
    ListModel* model() const { return m_model; }

    Q_INVOKABLE void sendPacket();
    Q_INVOKABLE void catchPacket();

private:
    ListModel *m_model = nullptr;
    QUdpSocket *senderSocket = nullptr;
    QUdpSocket *catcherSocket = nullptr;

private slots:
    void onReadyRead();
};

#endif // BACKEND_H
