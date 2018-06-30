#include "commandstop.h"

#include "core/server.h"

extern Server* server;
CommandStop::CommandStop(QString command) : Command(command)
{
    this->structure = "stop";
    this->description = "Stops the server";
    this->valid = validate();
}

bool CommandStop::validate()
{
    if(commandList.length() == 1) return true;

    this->errorMsg = "The 'stop' command format is: " + this->structure;
    return false;
}

bool CommandStop::execute()
{
    if(!valid) return false;

    server->stopServer();
    return true;
}
