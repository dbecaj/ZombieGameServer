#include "commandkick.h"

#include "core/server.h"

extern Server* server;
CommandKick::CommandKick(QString command) : Command(command)
{
    this->structure = "kick [playerName]";
    this->description = "Kicks the player off the game";
    this->valid = validate();
}

bool CommandKick::validate()
{
    if(commandList.length() == 2) return true;

    this->errorMsg = "The 'kick' command format is: " + this->structure;
    return false;
}

bool CommandKick::execute()
{
    if(!valid) return false;

    Player* player = server->getGame()->getPlayer(commandList[1]);
    if(!player)
    {
        errorMsg = "Player named '" + commandList[1] + "' is not connected or does not exist!";
        return false;
    }

    server->getGame()->wantToKick.push_back(player->getUsername());
    return true;
}
