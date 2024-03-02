#include "commandlist.h"

void CommandList::execute(QStringList args)
{
    if (args.size() != 1)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QList<QString> OnlineUsers = NetworkServer::getOnlineUsers();
    Message::logInfo(QString("Now %1 out of %2 users on the server").arg(OnlineUsers.size()).arg(MAXCONNECTIONS));
    Message::logInfo("Users: " + OnlineUsers.join(", "));
}

QString CommandList::getCommand()
{
    return "/list";
}

QString CommandList::getHelpInfo()
{
    return "Type /list to get a list of users";
}
