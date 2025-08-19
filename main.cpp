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

    FileDialogHelper fileDialogHelper;
    engine.rootContext()->setContextProperty("FileDialogHelper", &fileDialogHelper);
    // Register enum type for QML access
    qmlRegisterUncreatableType<StatusClass>("LAN.Backend", 1, 0, "StatusClass", "Not creatable as it is an enum type");


    // Handle QML load failure
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
                     &app, []() { QCoreApplication::exit(-1); },
                     Qt::QueuedConnection);

    engine.loadFromModule("LAN_fileshare", "Main");

    return app.exec();
}
