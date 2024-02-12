#include "commandmanager.h"

QMap<std::string, std::function<void(std::vector<std::string>)>> CommandManager::commandActions;

void CommandManager::init()
{
    std::thread handler(CommandHandler);
    handler.detach();

    //TODO: Создать классы самих команд (CommandBan, CommandKick, CommandHelp, CommandAddBalance) и из них как то
    //по хорошему в автоматическом режиме вытягивать функции к которым нужно обращаться при вводе этой команды
    //и соответственно кидать их в commandActions

    Message::logInfo("CommandManager started successfully!");
}

void CommandManager::CommandHandler()
{
    std::string command;

    while(true)
    {
        std::getline(std::cin, command);
        std::vector<std::string> args = parseCommand(command);

        if (!commandActions.contains(args[0]))
        {
            Message::logInfo("Unknown command, type /help for help");
            continue;
        }
        commandActions[args[0]](args);
    }
}

std::vector<std::string> CommandManager::parseCommand(std::string command)
{
    std::istringstream iss(command);
    std::vector<std::string> args;
    std::string argument;

    while (iss >> argument)
    {
        args.push_back(argument);
    }
    return args;
}

