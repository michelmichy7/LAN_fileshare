#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent}
{
    m_model = new ListModel(this);
}

void Backend::sendPacket()
{

}

void Backend::catchPacket()
{

}

void Backend::tcpConnection_REC(const QString &ip)
{

}

void Backend::tcpConnection_SEN(const QString &ip)
{

}

void Backend::transferFilesTCP()
{

}


