#include "updater.h"

void Updater::init()
{
    Ticker::addListener(QWeakPointer<Func>(pointerOnTick = QSharedPointer<Func>::create(std::bind(&Updater::onTick, this))));
}

void Updater::onTick()
{
    if (NetworkServer::getPlayerSockets().size() <= 0)
        return;

    startTablesUpdate();
}

void Updater::startTablesUpdate()
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QList<QSharedPointer<QSqlRecord>> result = databaseManager->executeQueryObjects("SELECT * FROM ActiveTables");

    deleteTable(result);

    for(QSharedPointer<QSqlRecord> responce : result)
    {
        int id = responce->value(0).toInt();
        double minBet = responce->value(2).toDouble();
        double stepBet = responce->value(3).toDouble();
        double minBalance = responce->value(4).toDouble();
        int maxNumPlayer = responce->value(1).toInt();
        QString nameGame = responce->value(5).toString();

        createOrUpdateTable(QSharedPointer<Game>(Game::getGame(nameGame)), TableSettings{id, minBet, stepBet, minBalance, maxNumPlayer});
    }
}

void Updater::createOrUpdateTable(QSharedPointer<Game> game, TableSettings tableSettings)
{
    if (game == nullptr)
        return;

    if (QSharedPointer<Table> table = Table::getTable(tableSettings.ID))
        table->setNewData(tableSettings);
    else
        Table::addTable(QSharedPointer<Table>(new Table(game, tableSettings)));
}

void Updater::deleteTable(const QList<QSharedPointer<QSqlRecord>>& newResult)
{
    QList<QSharedPointer<Table>>& oldTables = Table::getTabels();

    for (auto it = oldTables.begin(); it != oldTables.end(); it++)
    {
        bool found = false;
        for (const auto& currentTable : newResult)
        {
            if (currentTable->value(0).toInt() == it->get()->getSettings().ID)
            {
                found = true;
                break;
            }
        }

        if (!found)
            oldTables.erase(it);
    }
}


