#include "commandhelp.h"

void CommandHelp::execute(std::vector<std::string> args)
{
    if (args.size() != 1)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QString message{"Help by commands:\n"};

    for(Command* command : CommandManager::commands)
    {
        if (!command->getHelpInfo().empty())
            message += QString::fromStdString(command->getHelpInfo()) + "\n";
    }

    Message::logInfo(message);
}

std::string CommandHelp::getCommand()
{
    return "/help";
}

std::string CommandHelp::getHelpInfo()
{
    return "";
}
