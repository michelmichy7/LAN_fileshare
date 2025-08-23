#ifndef UDPSIDE_H
#define UDPSIDE_H

#include <QObject>
#include <QUdpSocket>
#include <QDebug>
#include <QNetworkInterface>


class Backend;
class ListModel;

class UDPManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ListModel* model READ model CONSTANT)

public:
    QStringList getLocalIPs() const;

    explicit UDPManager(Backend* backend, QObject *parent = nullptr);
    ListModel* model() const { return m_model; }
    Q_INVOKABLE void catchPacket();
    Q_INVOKABLE void sendPacket(const QString &datagram, QHostAddress ip = QHostAddress::Broadcast);

    QString ipOF_HOST;
    QString getPreferredLocalIP() const;

    Q_INVOKABLE void readLocalIps() {
        for (int i = 0; i < m_localIPs.size(); i++) {
            qDebug() << m_localIPs[i];
        }
    }

signals:
    void showConnectionPage(const QString &message);
    void tcpConnected(const QString &ip);

private:
    QUdpSocket *senderSocket = nullptr;

    Backend* m_backend = nullptr;
    ListModel* m_model = nullptr;


    QStringList m_localIPs;


public slots:
    void sendStatusPacket(const QString& ip);


private slots:
    void onDoConnectionBox(const QString &ip);

    void onReadyRead();
};

#endif // UDPSIDE_H
