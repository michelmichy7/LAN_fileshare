#include "Backend/backend.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<Backend>("Manager", 1, 0, "Backend");
    QQmlApplicationEngine engine;
<<<<<<< HEAD
    ui->
=======
>>>>>>> 1fabce4f2a88104eb18d13ba9862a44c6d0aa14d
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("LAN_fileshare", "Main");

    return app.exec();
}
