#include "commandplayers.h"

#include "core/server.h"

extern Server* server;
CommandPlayers::CommandPlayers(QString command) : Command(command)
{
    this->structure = "players";
    this->description = "Shows all connected players";
    this->valid = validate();
}

bool CommandPlayers::validate()
{
    if(commandList.length() == 1) return true;

    this->errorMsg = "The 'players' command format is: " + this->structure;
    return false;
}

bool CommandPlayers::execute()
{
    if(!valid) return false;

    QString message = "";
    for(Player* player : server->getGame()->players)
        message = player->getUsername() + ", ";
    server->getLogger()->output(message);

    return true;
}
