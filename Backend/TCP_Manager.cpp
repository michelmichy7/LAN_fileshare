#include "Backend.h"

#include "tcpside.h"

#include <QFile>
#include <qfileinfo.h>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>


TCPManager::TCPManager(Backend* backend, QObject *parent)
    : QObject(parent), m_backend(backend)
{
    tcpServer = nullptr;
    tcpSocket = nullptr;
}

void TCPManager::startServer(quint16 port)
{
    tcpServer = new QTcpServer(this);

    connect(tcpServer, &QTcpServer::newConnection, this, &::TCPManager::onNewConnection);

    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Server could not start! Error:" << tcpServer->errorString();
        return;
    }

    qDebug() << "Server started on port" << port;
}

void TCPManager::connectToHost(const QHostAddress &ip, quint16 port = 45454)
{
    if (m_backend->connectionState() == StatusClass::TCP_CONNECTED) {
        qDebug() << "TCP: Another device tried connection, declined - already connected to another";
        return;
    }

    if (!tcpSocket) {
        tcpSocket = new QTcpSocket(this);

        connect(tcpSocket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);
        connect(tcpSocket, &QTcpSocket::connected, this, [this]() {
            qDebug() << "Connected to server!";
            m_backend->setConnectionState(StatusClass::TCP_CONNECTED);
        });
    }

    tcpSocket->connectToHost(ip, port);
}


void TCPManager::sendData()
{
    qDebug("Sending data...");
    const auto &files = m_backend->m_filesManager.m_selectedFiles;

    for (const QString &filePath : files) {
        sendFile(filePath);  // use the dedicated function for one file
    }
}

void TCPManager::sendFile(const QString &filePath)
{
    qDebug() << "Sending file:" << filePath;

    if (!tcpSocket || tcpSocket->state() != QTcpSocket::ConnectedState) {
        qDebug() << "Not connected to any host.";
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file:" << filePath;
        return;
    }

    // Prepare FileHeader
    FileHeader header;
    QFileInfo fileInfo(filePath);
    header.fileName = fileInfo.fileName();
    header.fileSize = file.size();
    header.created = fileInfo.birthTime(); // Qt 5.10+ and Qt 6+

    header.mimeType = QMimeDatabase().mimeTypeForFile(fileInfo).name(); // optional

    // Send the header
    QDataStream out(tcpSocket);
    out.setVersion(QDataStream::Qt_6_9); // Or whatever version you're using
    out << header;

    // Send file data in chunks (avoid large memory usage)
    const int chunkSize = 64 * 1024; // 64KB
    QByteArray buffer;
    while (!file.atEnd()) {
        buffer = file.read(chunkSize);
        tcpSocket->write(buffer);
        tcpSocket->flush(); // ensure it's sent
        if (!tcpSocket->waitForBytesWritten(-1)) {
            qDebug() << "Failed to write data.";
            break;
        }
    }

    file.close();
    qDebug() << "File sent successfully:" << header.fileName;
}



void TCPManager::onNewConnection()
{
    if (!m_clientSockets.isEmpty()) {
        // Already have a client, reject the new connection immediately
        QTcpSocket *newSocket = tcpServer->nextPendingConnection();
        qDebug() << "Rejected new client from" << newSocket->peerAddress().toString() << "because a client is already connected";
        newSocket->disconnectFromHost();
        newSocket->deleteLater();
        return;
    }

    // No clients connected — accept this new connection
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, &TCPManager::onReadyRead);
    connect(clientSocket, &QTcpSocket::disconnected, this, &TCPManager::onDisconnected);

    // Store the client socket if you want to communicate with it later
    m_clientSockets.append(clientSocket);

    qDebug() << "New client connected from" << clientSocket->peerAddress().toString();
}
void TCPManager::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    static QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_5);

    static FileHeader currentHeader;
    static QFile currentFile;
    static qint64 bytesReceived = 0;
    static bool headerRead = false;

    while (true) {
        if (!headerRead) {
            if (in.atEnd()) return;

            // Try reading header
            in >> currentHeader;
            if (in.status() != QDataStream::Ok) return;

            headerRead = true;
            bytesReceived = 0;

            // Prepare save path
            QString savePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                               + "/NearbyFiles/" + currentHeader.fileName;
            QDir().mkpath(QFileInfo(savePath).absolutePath());

            currentFile.setFileName(savePath);
            if (!currentFile.open(QIODevice::WriteOnly)) {
                qDebug() << "Failed to open file for writing:" << savePath;
                headerRead = false;
                return;
            }

            qDebug() << "Receiving file:" << currentHeader.fileName
                     << "Size:" << currentHeader.fileSize;
        }

        // Now read file data
        while (socket->bytesAvailable() > 0 && bytesReceived < currentHeader.fileSize) {
            QByteArray chunk = socket->read(qMin(currentHeader.fileSize - bytesReceived, qint64(64 * 1024)));
            currentFile.write(chunk);
            bytesReceived += chunk.size();
        }

        if (bytesReceived >= currentHeader.fileSize) {
            currentFile.close();
            qDebug() << "✅ File received successfully:" << currentHeader.fileName;

            headerRead = false;
            return; // done with this file — wait for next read
        } else {
            return; // wait for more data
        }
    }
}




void TCPManager::onDisconnected()
{
    m_backend->setConnectionState(StatusClass::ConnectionState::TCP_DISCONNECTED);
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "Client disconnected:" << socket->peerAddress().toString();

    m_clientSockets.removeAll(socket);
    socket->deleteLater();
}
