#include "Backend/Backend.h"
#include "Backend/tcpside.h"
#include "Backend/udpside.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);
    engine.rootContext()->setContextProperty("listModel", backend.model());

    TCPServer tcpServer;
    engine.rootContext()->setContextProperty("tcpServer", &tcpServer);
    TCPClient tcpClient(&backend);
    engine.rootContext()->setContextProperty("tcpClient", &tcpClient);

    UDPSender udpSender;
    engine.rootContext()->setContextProperty("udpSender", &udpSender);

    UDPSender udpReceiver;
    engine.rootContext()->setContextProperty("udpReceiver", &udpReceiver);

    FileDialogHelper fileDialogHelper;
    engine.rootContext()->setContextProperty("FileDialogHelper", &fileDialogHelper);


    QObject::connect(&backend, &Backend::tcpConnected,
                     &udpSender, &UDPSender::sendStatusPacket);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("LAN_fileshare", "Main");

    return app.exec();
}
