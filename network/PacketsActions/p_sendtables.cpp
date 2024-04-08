#include "p_sendtables.h"

const PacketTypes P_SendTables::packettype = PacketTypes::P_SendTables;

void P_SendTables::sendTables(QSharedPointer<SOCKET> clientSocket)
{
    Table::tables.clear();

//    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
//    QList<QSharedPointer<QSqlQuery>> responces = databaseManager->executeQueryObjects("SELECT * FROM ActiveTables;");

//    for(QSharedPointer<QSqlQuery> responce : responces)
//        qDebug() << responce->value(0).toString();


    QSharedPointer<Table> t1(new Table(Game("Блэкджек"), TableSettings{1, 11, 12, 13, 4}));
    Table::addTable(t1);

    QSharedPointer<Table> t2(new Table(Game("Покер"), TableSettings{2, 21, 22, 23, 8}));
    Table::addTable(t2);

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
