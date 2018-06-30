#include "commandreset.h"

#include <random>
#include <ctime>

#include "core/server.h"
#include "game/gamemp.h"
#include "game/entities/mobs/zombie.h"
#include "game/entities/mobs/scientist.h"
#include "game/entities/mobs/player.h"
#include "packets/packetentitydata.h"

extern Server* server;
CommandReset::CommandReset(QString command) : Command(command)
{
    this->structure = "reset";
    this->description = "Resets the game";
    this->valid = validate();
}

bool CommandReset::validate()
{
    if(commandList.length() == 1) return true;

    this->errorMsg = "The 'reset' command structure is: " + this->structure;
    return false;
}

bool CommandReset::execute()
{
    if(!valid) return false;

    server->getGame()->wantToReset = true;

    return true;
}
