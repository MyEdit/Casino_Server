#include "p_sendtables.h"

const PacketTypes P_SendTables::packettype = PacketTypes::P_SendTables;

void P_SendTables::sendTables(QSharedPointer<SOCKET> clientSocket)
{
    Table::tables.clear();

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QList<QSharedPointer<QSqlRecord>> result = databaseManager->executeQueryObjects("SELECT * FROM ActiveTables");

    for(QSharedPointer<QSqlRecord> responce : result)
    {
        int id = responce->value(0).toInt();
        double minBet = responce->value(3).toDouble();
        double stepBet = responce->value(4).toDouble();
        double minBalance = responce->value(5).toDouble();
        int maxNumPlayer = responce->value(1).toInt();
        QString nameGame = responce->value(6).toString();

        QSharedPointer<Table> table(new Table(Game(nameGame), TableSettings{id, minBet, stepBet, minBalance, maxNumPlayer}));
        Table::addTable(table);
    }

    int countTable = Table::tables.size();

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &countTable, sizeof(int));

    for(QSharedPointer<Table> table : Table::tables)
    {
        QByteArray jsonData = table->serializeTable();
        int dataSize = jsonData.size();
        NetworkServer::sendToClient(clientSocket, &dataSize, sizeof(int));
        NetworkServer::sendToClient(clientSocket, jsonData.data(), dataSize);
    }
}
