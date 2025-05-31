#include "backend.h"
#include "QDebug"


Backend::Backend(QObject *parent)
    : QObject{parent}
{}

void Backend::find_Lan_Devices() {
    qDebug() << "hello world";
}
