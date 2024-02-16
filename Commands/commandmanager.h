#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <thread>
#include <iostream>
#include <sstream>
#include <Utils/Message.h>
#include <Commands/command.h>
#include <Commands/commandhelp.h>
#include <Commands/commandban.h>

class CommandManager
{
private:
    static std::vector<Command*> commands;
    static QMap<std::string, std::function<void(std::vector<std::string>)>> commandActions;

    void registerCommands();
    static void CommandHandler();
    static std::vector<std::string> parseCommand(std::string command);

    friend class CommandHelp;

public:
    void init();
};

#endif // COMMANDMANAGER_H
