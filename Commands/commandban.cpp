#include "commandban.h"

void CommandBan::execute(QStringList args)
{
    if (args.size() < 3)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString subQuery = QString("SELECT ID_User FROM Users WHERE Login = '%1'").arg(args[1]);
    QString id_user = databaseManager->executeQuery(subQuery);
    QString reason = Command::getTextAfterIndex(args, 2);

    if (id_user == nullptr)
    {
        Message::logInfo("Player " + args[1] + " not found");
        return;
    }

    QString query = QString("INSERT INTO Banlist (ID_User, Reason) VALUES (%1, '%2')").arg(id_user).arg(reason);

    if (!databaseManager->executeQueryWithoutResponce(query))
    {
        Command::printErrorMessage();
        return;
    }
    NetworkServer::onClientDisconnected(NetworkServer::getSocketByNickname(args[1]));
    Message::logInfo("Player " + args[1] + " successfully banned");
}

QString CommandBan::getCommand()
{
    return "/ban";
}

QString CommandBan::getHelpInfo()
{
    return "Type /ban <Nickname> <Reason> to ban player";
}
