#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <thread>
#include <iostream>
#include <sstream>
#include <Utils/Message.h>
#include <Commands/command.h>
#include <Commands/commandhelp.h>
#include <Commands/commandban.h>
#include <Commands/commandnotification.h>
#include <Commands/commandlist.h>

class CommandManager
{
private:
    static std::vector<Command*> commands;
    static QMap<QString, std::function<void(QStringList)>> commandActions;

    void registerCommands();
    static void CommandHandler();
    static QStringList parseCommand(QString command);

    friend class CommandHelp;

public:
    void init();
    static void registerCommand(Command* command);
};

#endif // COMMANDMANAGER_H
