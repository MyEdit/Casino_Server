#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <thread>
#include <iostream>
#include <sstream>
#include <Utils/Message.h>

class CommandManager
{
private:
    static QMap<std::string, std::function<void(std::vector<std::string>)>> commandActions;

    static void CommandHandler();
    static std::vector<std::string> parseCommand(std::string command);

public:
    void init();
};

#endif // COMMANDMANAGER_H
