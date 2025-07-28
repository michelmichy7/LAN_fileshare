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

    engine.rootContext()->setContextProperty("udpManager", backend.udpManager());

    // You had udpReceiver commented out
    /*
    UDPSender udpReceiver(&backend);
    engine.rootContext()->setContextProperty("udpReceiver", &udpReceiver);
    */

    FileDialogHelper fileDialogHelper;
    engine.rootContext()->setContextProperty("FileDialogHelper", &fileDialogHelper);
    // Register enum type for QML access
    qmlRegisterUncreatableType<StatusClass>("LAN.Backend", 1, 0, "StatusClass", "Not creatable as it is an enum type");

    // Connect signals
   /* QObject::connect(&backend, &Backend::tcpConnected,
                     qobject_cast<UDPManager*>(backend.udpManager()), &UDPManager::sendStatusPacket);*/


    // Handle QML load failure
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.loadFromModule("LAN_fileshare", "Main");

    return app.exec();
}
