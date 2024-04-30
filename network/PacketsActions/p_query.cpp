#include "p_query.h"

void P_Query::getResultQuary(QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());

    QueryTypes queryTypes = NetworkServer::getMessageFromClient<QueryTypes>(clientSocket);
    ModelTypes modelTypes = NetworkServer::getMessageFromClient<ModelTypes>(clientSocket);
    QString request = NetworkServer::getMessageFromClient(clientSocket);

    sendResult(clientSocket, databaseManager->executeQuery(request), modelTypes, queryTypes);
}

void P_Query::sendResult(QSharedPointer<SOCKET> clientSocket, const QString& result, const ModelTypes modelTypes, const QueryTypes queryTypes)
{
    PacketTypes packettype = PacketTypes::P_QueryWithoutResponce;

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modelTypes, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, &queryTypes, sizeof(QueryTypes));
    NetworkServer::sendToClient(clientSocket, result);
}


