#include "p_query.h"

void P_Query::getResultQuary(QSharedPointer<SOCKET> clientSocket)
{
    if(P_SendModel::tableNames.isEmpty())
        P_SendModel::initMapTableNames();

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());

    QueryTypes queryTypes;
    ModelTypes modelTypes;

    recv(*clientSocket, reinterpret_cast<char*>(&queryTypes), sizeof(QueryTypes), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&modelTypes), sizeof(ModelTypes), 0);
    QString request = NetworkServer::getMessageFromClient(clientSocket);

    sendResult(clientSocket, databaseManager->executeQuery(request), modelTypes, queryTypes);
}

void P_Query::sendResult(QSharedPointer<SOCKET> clientSocket, QString result, ModelTypes modelTypes, QueryTypes queryTypes)
{
    PacketTypes packettype = PacketTypes::P_QueryWithoutResponce;

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modelTypes, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, &queryTypes, sizeof(QueryTypes));
    NetworkServer::sendToClient(clientSocket, result);
}


