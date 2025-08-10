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
}// =======================
// Sending side
// =======================
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
    header.created = fileInfo.birthTime();
    header.mimeType = QMimeDatabase().mimeTypeForFile(fileInfo).name();

    // Serialize header
    QByteArray headerBlock;
    {
        QDataStream headerStream(&headerBlock, QIODevice::WriteOnly);
        headerStream.setVersion(QDataStream::Qt_6_9);
        headerStream << header;
    }

    // Send header size (int32)
    qint32 headerSize = headerBlock.size();
    QByteArray sizePrefix;
    {
        QDataStream sizeStream(&sizePrefix, QIODevice::WriteOnly);
        sizeStream.setVersion(QDataStream::Qt_6_9);
        sizeStream << headerSize;
    }

    tcpSocket->write(sizePrefix);
    tcpSocket->write(headerBlock);

    // Send file data in chunks
    const int chunkSize = 64 * 1024;
    QByteArray buffer;
    while (!file.atEnd()) {
        buffer = file.read(chunkSize);
        tcpSocket->write(buffer);
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
}// =======================
// Receiving side
// =======================
void TCPManager::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    // Static vars keep state between readyRead calls
    static qint32 expectedHeaderSize = -1;
    static FileHeader currentHeader;
    static QFile currentFile;
    static qint64 bytesReceived = 0;

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_9);

    while (true) {
        // Step 1: Read header size (4 bytes)
        if (expectedHeaderSize == -1) {
            if (socket->bytesAvailable() < sizeof(qint32))
                return; // wait for full size
            in >> expectedHeaderSize;
            continue; // loop to try reading header immediately
        }

        // Step 2: Read header block
        if (currentHeader.fileName.isEmpty()) {
            if (socket->bytesAvailable() < expectedHeaderSize)
                return; // wait for full header

            QByteArray headerBlock = socket->read(expectedHeaderSize);
            QDataStream headerStream(&headerBlock, QIODevice::ReadOnly);
            headerStream.setVersion(QDataStream::Qt_6_9);
            headerStream >> currentHeader;

            // Prepare file to save
            QString savePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)
                               + "/NearbyFiles/" + currentHeader.fileName;
            QDir().mkpath(QFileInfo(savePath).absolutePath());

            currentFile.setFileName(savePath);
            if (!currentFile.open(QIODevice::WriteOnly)) {
                qDebug() << "Failed to open file for writing:" << savePath;
                // Reset for next transfer
                expectedHeaderSize = -1;
                currentHeader = FileHeader();
                return;
            }

            bytesReceived = 0;
            qDebug() << "Receiving file:" << currentHeader.fileName
                     << "Size:" << currentHeader.fileSize;
            continue; // loop to try reading file immediately
        }

        // Step 3: Read file data
        if (bytesReceived < currentHeader.fileSize) {
            QByteArray chunk = socket->read(qMin(currentHeader.fileSize - bytesReceived, qint64(64 * 1024)));
            currentFile.write(chunk);
            bytesReceived += chunk.size();

            if (bytesReceived < currentHeader.fileSize)
                return; // wait for more data
        }

        // Step 4: File complete
        if (bytesReceived >= currentHeader.fileSize) {
            currentFile.close();
            qDebug() << "✅ File received successfully:" << currentHeader.fileName;

            // Reset for next file
            expectedHeaderSize = -1;
            currentHeader = FileHeader();
            bytesReceived = 0;
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
