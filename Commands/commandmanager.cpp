#include "commandmanager.h"

QMap<std::string, std::function<void(std::vector<std::string>)>> CommandManager::commandActions;

void CommandManager::init()
{
    registerCommands();

    for(Command* command : commands)
    {
        commandActions.insert(command->getCommand(), [&](std::vector<std::string> args)
        {
            command->execute(args);
        });
    }

    std::thread handler(CommandHandler);
    handler.detach();

    Message::logInfo("CommandManager started successfully!");
}

void CommandManager::registerCommands()
{
    commands.push_back(new CommandHelp());
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

void CommandManager::CommandHandler()
{
    std::string command;

    while(true)
    {
        std::getline(std::cin, command);
        std::vector<std::string> args = parseCommand(command);

        if (args.size() == 0 || !commandActions.contains(args[0]))
        {
            Message::logInfo(Command::getErrorMessage());
            continue;
        }
        commandActions[args[0]](args);
    }
}

