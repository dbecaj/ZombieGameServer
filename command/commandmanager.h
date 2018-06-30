#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QString>
#include <QList>

#include "command.h"

class CommandManager
{
    QList<Command*> commands;
public:
    CommandManager();
    ~CommandManager();

    void parse(QString command);
    void showHelp();
    void describeCommand(Command* command);
};

#endif // COMMANDMANAGER_H
