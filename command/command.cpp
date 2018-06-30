#include "command.h"

Command::Command(QString command)
{
    commandList = command.split(" ");
    structure = "";
    description = "";
    errorMsg = "";
    valid = false;
}
