#include <QCoreApplication>
#include <iostream>
#include <QHostAddress>
#include <string>
#include <QString>
#include <QDebug>

#include "core/server.h"

Server* server = nullptr;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    server = new Server(&a);
    if(!server->init()) return -1;
    server->bind(SERVER_PORT);

    return a.exec();
}
