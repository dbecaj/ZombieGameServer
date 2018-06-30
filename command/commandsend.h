#ifndef COMMANDSEND_H
#define COMMANDSEND_H

#include "command.h"

class CommandSend : public Command
{
public:
    CommandSend(QString command);

    bool validate();
    bool execute();
};

#endif // COMMANDSEND_H
