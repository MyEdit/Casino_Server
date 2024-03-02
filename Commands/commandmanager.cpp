#include "commandmanager.h"

QMap<QString, std::function<void(QStringList)>> CommandManager::commandActions;
std::vector<Command*> CommandManager::commands;

void CommandManager::init()
{
    registerCommands();
    std::thread handler(CommandHandler);
    handler.detach();
}

void CommandManager::registerCommands()
{
    commands.push_back(new CommandHelp());
    commands.push_back(new CommandBan());
    commands.push_back(new CommandNotification());
    commands.push_back(new CommandList());

    for(Command* command : commands)
    {
        commandActions.insert(command->getCommand(), [command](QStringList args)
        {
            command->execute(args);
        });
        Message::logInfo("Command " + command->getCommand() + " successfully registered");
    }
}

QStringList CommandManager::parseCommand(QString command)
{
    return command.split(" ", QString::SkipEmptyParts);
}

void CommandManager::CommandHandler()
{
    std::string command;

    while(true)
    {
        std::getline(std::cin, command);
        QStringList args = parseCommand(QString::fromStdString(command));

        if (args.size() == 0 || !commandActions.contains(args[0]))
        {
            Command::printUnknownCommandMessage();
            continue;
        }
        commandActions[args[0]](args);
    }
}

