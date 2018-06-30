#include "commandsend.h"

#include "core/server.h"
#include "packets/packetbasic.h"

extern Server* server;
CommandSend::CommandSend(QString command) : Command(command)
{
    this->structure = "send [playerName] [message]";
    this->description = "Sends a BASIC message to the player";
    this->valid = validate();
}

bool CommandSend::validate()
{
    if(commandList.length() >= 3) return true;

    errorMsg = "The 'send' command format is: " + this->structure;
    return false;
}

bool CommandSend::execute()
{
    if(!valid) return false;

    Player* player = server->getGame()->getPlayer(commandList[1]);
    if(!player)
    {
        errorMsg = "Player named '" + commandList[1] + "' is not connected or does not exist!";
        return false;
    }

    QString message = "";
    for(int i = 2; i < commandList.length(); i++)
        message = commandList[i] + " ";
    message = message.mid(message.length() - 1);
    PacketBasic packet(message);
    player->sendPacket(&packet);

    return true;
}
