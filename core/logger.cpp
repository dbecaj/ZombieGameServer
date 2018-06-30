#include "logger.h"

#include <iostream>
#include <QTextStream>

Logger::Logger(QObject *parent) : QObject(parent)
{
    hasInputTextHeader = false;
}

void Logger::run()
{
    this->moveToThread(&thread);
    this->connect(&thread, SIGNAL(started()), this, SLOT(readCommands()));

    thread.start();
}

void Logger::stop()
{
    if(thread.isRunning()) thread.quit();
}

void Logger::readCommands()
{
    std::string command = "";
    while(command != "stop")
    {
        if(!hasInputTextHeader)
        {
            std::cout << "SERVER > ";
            hasInputTextHeader = true;
        }
        std::getline(std::cin, command);
        hasInputTextHeader = false;
        if(command == "") continue;
        commandManager.parse(QString::fromStdString(command));
    }
}

void Logger::output(QString str, MessageType type, bool header)
{
    std::string typeHeader = "INFO";
    if(type == MessageType::WARNING) typeHeader = "WARNING";
    else if(type == MessageType::ERROR) typeHeader = "ERROR";

    if(header) std::cout << "\r" << "SERVER[" + typeHeader + "] : ";
    else std::cout << "\r";
    std::cout << str.toStdString() << std::endl;
    hasInputTextHeader = false;

    // If we don't have the header write it down because we all always requesting the input
    if(!hasInputTextHeader)
    {
        std::cout << "SERVER > ";
        hasInputTextHeader = true;
    }
}
