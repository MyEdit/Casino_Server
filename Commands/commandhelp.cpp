#include "commandhelp.h"

void CommandHelp::execute(std::vector<std::string> args)
{
    if (args.size() != 1)
    {
        Message::logInfo(Command::getErrorMessage());
        return;
    }

    Message::logInfo
    (
        "Help by commands:\n"
        "Type /ban <NickName> to ban player\n"
        "Type /unban <NickName> to unban player\n"
        "Type /kick <NickName> to kick player\n"
        "Type /addbalance <NickName> <Count> to add player balance"
     );
}

std::string CommandHelp::getCommand()
{
    return "/help";
}
