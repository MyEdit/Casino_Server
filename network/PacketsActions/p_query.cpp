#include "p_query.h"

QMap<QueryTypes, QString> P_Query::querys;

void P_Query::getResultQuary(QSharedPointer<SOCKET> clientSocket)
{
    if(querys.isEmpty())
        initMapQuerys();

    if(P_SendModel::tableNames.isEmpty())
        P_SendModel::initMapTableNames();

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());

    QueryTypes queryTypes;
    ModelTypes modelTypes;

    recv(*clientSocket, reinterpret_cast<char*>(&queryTypes), sizeof(QueryTypes), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&modelTypes), sizeof(ModelTypes), 0);

    QString query = querys[queryTypes] + P_SendModel::tableNames[modelTypes];
    sendResult(clientSocket, databaseManager->executeQuery(query));
}

void P_Query::sendResult(QSharedPointer<SOCKET> clientSocket, QString result)
{
    PacketTypes packettype = PacketTypes::P_QueryWithoutResponce;

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, result);
}

void P_Query::initMapQuerys()
{
    querys.insert(QueryTypes::CountEntrites, "SELECT COUNT(*) FROM ");
}
