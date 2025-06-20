#include "Backend/Backend.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);
    ListModel m_model;
    engine.rootContext()->setContextProperty("listModel", &m_model);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("LAN_fileshare", "Main");

    return app.exec();
}
