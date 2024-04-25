#include "commandhelp.h"

void CommandHelp::execute(const QStringList& args)
{
    if (args.size() != 1)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QString message{"Help by commands:\n"};

    for(QSharedPointer<Command> command : CommandManager::commands)
    {
        if (!command->getHelpInfo().isNull())
            message += command->getHelpInfo() + "\n";
    }

    Message::logInfo(message);
}

QString CommandHelp::getCommand()
{
    return "/help";
}

QString CommandHelp::getHelpInfo()
{
    return NULL;
}
