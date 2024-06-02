#include "commandtableinfo.h"

void CommandTableInfo::execute(const QStringList& args)
{
    if (args.size() < 2)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QSharedPointer<Table> table = Table::getTable(args[1].toInt());
    if (!table)
    {
        Message::logError("Table with ID = " + args[1] + " not found");
        return;
    }

    TableSettings tableSettings = table->getSettings();
    QSharedPointer<Game> game = table->getGame();

    Message::logInfo("Table ID: " + args[1]);
    Message::logInfo("- Game: " + game->getName());
    Message::logInfo("- Game status: " + QString(game->isGameRunning() ? "Running" : "Wait players"));
    Message::logInfo("- Min bet: " + QString::number(tableSettings.minBet));
    Message::logInfo("- Step bet: " + QString::number(tableSettings.stepBet));
    Message::logInfo("- Min balance: " + QString::number(tableSettings.minBalance));
    Message::logInfo("- Max count players: " + QString::number(tableSettings.maxCountPlayers));

    QStringList playerNames;
    for (QSharedPointer<Player> player : table->getPlayers())
    {
        if (player)
        {
            playerNames << player->getLogin();
        }
    }

    Message::logInfo("- Players: " + playerNames.join(","));
}

QString CommandTableInfo::getCommand()
{
    return "/tableinfo";
}

QString CommandTableInfo::getHelpInfo()
{
    return "Type /tableinfo <Table ID> to get info about table";
}
