#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QUdpSocket>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE void find_Lan_Devices();

private:
    QUdpSocket *senderSocket = nullptr;

signals:
};

#endif // BACKEND_H
