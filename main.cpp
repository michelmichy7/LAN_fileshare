#include "Backend/Backend.h"
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

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("LAN_fileshare", "Main");

    return app.exec();
}
