#include "commandmanager.h"

QMap<QString, std::function<void(QStringList&)>> CommandManager::commandActions;
std::vector<QSharedPointer<Command>> CommandManager::commands;

void CommandManager::init()
{
    registerCommands();
    std::thread handler(CommandHandler);
    handler.detach();
}

void CommandManager::registerCommands()
{
    commands.push_back(QSharedPointer<Command>(new CommandHelp));
    commands.push_back(QSharedPointer<Command>(new CommandHelp()));
    commands.push_back(QSharedPointer<Command>(new CommandBan()));
    commands.push_back(QSharedPointer<Command>(new CommandNotification()));
    commands.push_back(QSharedPointer<Command>(new CommandList()));
    commands.push_back(QSharedPointer<Command>(new CommandTableInfo()));

    for(QSharedPointer<Command> command : commands)
    {
        commandActions.insert(command->getCommand(), [command](QStringList& args)
        {
            command->execute(args);
        });
        Message::logInfo("Command " + command->getCommand() + " successfully registered");
    }
}

QStringList CommandManager::parseCommand(const QString& command)
{
    return command.split(" ", QString::SkipEmptyParts);
}

void CommandManager::CommandHandler()
{
    QTextStream in(stdin);
    QString command;

    while(true)
    {
        command = in.readLine();
        QStringList args = parseCommand(command);

        if (args.size() == 0 || !commandActions.contains(args[0]))
        {
            Command::printUnknownCommandMessage();
            continue;
        }
        commandActions[args[0]](args);
    }
}

