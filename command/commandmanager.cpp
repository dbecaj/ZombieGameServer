#include "commandmanager.h"

#include <QDebug>

#include "core/server.h"
#include "commandsend.h"
#include "commandkick.h"
#include "commandplayers.h"
#include "commandstop.h"
#include "commandreset.h"

extern Server* server;
CommandManager::CommandManager()
{
    commands.push_back(new CommandSend("send Pepe"));
    commands.push_back(new CommandKick("kick Pepe"));
    commands.push_back(new CommandPlayers("players"));
    commands.push_back(new CommandReset("reset"));
    commands.push_back(new CommandStop("stop"));
}

CommandManager::~CommandManager()
{
    for(Command* command : commands)
        delete command;
}

void CommandManager::parse(QString command)
{
    bool describe = false;
    if(command[command.length() - 1] == QChar('?'))
    {
        command = command.mid(0, command.length() - 1);
        describe = true;
    }

    Command* c = nullptr;
    QString com = command.split(" ")[0];
    if(com == "help") showHelp();
    else if(com == "send") c = new CommandSend(command);
    else if(com == "kick") c = new CommandKick(command);
    else if(com == "players") c = new CommandPlayers(command);
    else if(com == "reset") c = new CommandReset(command);
    else if(com == "stop") c = new CommandStop(command);
    else if(com == ":3") server->getLogger()->output("meow");
    else if(com == "Hello") server->getLogger()->output("Sorry I have a Boyfriend.");
    else
    {
        server->getLogger()->output(command + " does not exist!", MessageType::ERROR);
        return;
    }

    if(!c) return;

    if(describe) describeCommand(c);
    else if(!c->execute()) server->getLogger()->output(c->getErrorMsg(), MessageType::WARNING);
}

void CommandManager::showHelp()
{
    QString help = "\nHere are all the commands and their descriptions:\n";
    for(Command* command : commands)
    {
        help += "  " + command->getStructure().split(" ")[0];
        int spaces = 9 - command->getStructure().split(" ")[0].length();
        for(int i = 0; i < spaces; i++)
            help += " ";
        help += command->getDescription() + "\n";
    }
    help += "\nIf you want to see more detailed description of the certain command just type '?' at the end of it\n";
    server->getLogger()->output(help, MessageType::INFO, false);
}

void CommandManager::describeCommand(Command *command)
{
    QString description = "  " + command->getDescription() + "\n";
    description += "  Usage: " + command->getStructure();
    server->getLogger()->output(description, MessageType::INFO, false);
}
