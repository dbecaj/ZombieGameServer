#ifndef COMMANDPLAYERS_H
#define COMMANDPLAYERS_H

#include "command.h"

class CommandPlayers : public Command
{
public:
    CommandPlayers(QString command);

    bool validate();
    bool execute();
};

#endif // COMMANDPLAYERS_H
