#include "commandban.h"

void CommandBan::execute(std::vector<std::string> args)
{
    if (args.size() != 3)
    {
        Message::logInfo(Command::getUnknownCommandMessage());
        return;
    }

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString subQuery = QString("SELECT ID_User FROM Users WHERE Login = '%1'").arg(QString::fromStdString(args[1]));
    QString id_user = databaseManager->executeQuery(subQuery);

    if (id_user == nullptr)
    {
        Message::logInfo("Player " + QString::fromStdString(args[1]) + " not found");
        return;
    }

    QString query = QString("INSERT INTO Banlist (ID_User, Reason) VALUES (%1, '%2')").arg(id_user).arg(QString::fromStdString(args[2]));

    if (!databaseManager->executeQueryWithoutResponce(query))
    {
        Command::printErrorMessage();
        return;
    }

    Message::logInfo("Player " + QString::fromStdString(args[1]) + " successfully banned");
}

std::string CommandBan::getCommand()
{
    return "/ban";
}

std::string CommandBan::getHelpInfo()
{
    return "Type /ban <Nickname> <Reason> to ban player";
}
