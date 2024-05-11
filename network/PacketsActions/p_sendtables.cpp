﻿#include "p_sendtables.h"

const PacketTypes P_SendTables::packettype = PacketTypes::P_SendTables;

//TODO: Убрать обновление столов в один отдельный поток
void P_SendTables::sendTables(QSharedPointer<SOCKET> clientSocket)
{
    auto createOrUpdateTable = [](QSharedPointer<Game> game, TableSettings tableSettings)
    {
        if (game == nullptr)
            return;

        if (QSharedPointer<Table> table = Table::getTable(tableSettings.ID))
            table->setNewData(tableSettings);
        else
            Table::addTable(QSharedPointer<Table>(new Table(game, tableSettings)));
    };

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

    int countTable = Table::getCopyListTabels().size();

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &countTable, sizeof(int));

    for(QSharedPointer<Table> table : Table::getCopyListTabels())
    {
        QByteArray jsonData = table->serializeTable();
        int dataSize = jsonData.size();
        NetworkServer::sendToClient(clientSocket, &dataSize, sizeof(int));
        NetworkServer::sendToClient(clientSocket, jsonData.data(), dataSize);
    }
}

void P_SendTables::deleteTable(const QList<QSharedPointer<QSqlRecord>>& newResult)
{
    QList<QSharedPointer<Table>>& oldTables = Table::getTabels();

    for (auto it = oldTables.begin(); it != oldTables.end();)
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
        {
            it->get()->throwOutPlayers();
            oldTables.erase(it);
        }
        else
            ++it;
    }
}
