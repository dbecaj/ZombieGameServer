#ifndef COMMANDRESET_H
#define COMMANDRESET_H

#include "command.h"

class CommandReset : public Command
{
public:
    CommandReset(QString command);

    bool validate();
    bool execute();
};

#endif // COMMANDRESET_H
