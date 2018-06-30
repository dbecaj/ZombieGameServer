#ifndef COMMANDSTOP_H
#define COMMANDSTOP_H

#include "command.h"

class CommandStop : public Command
{
public:
    CommandStop(QString command);

    bool validate();
    bool execute();
};

#endif // COMMANDSTOP_H
