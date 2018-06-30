#ifndef COMMANDKICK_H
#define COMMANDKICK_H

#include "command.h"

class CommandKick : public Command
{
public:
    CommandKick(QString command);

    bool validate();
    bool execute();
};

#endif // COMMANDKICK_H
