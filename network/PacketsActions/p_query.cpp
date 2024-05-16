#include "p_query.h"

void P_Query::getResultQuary(QSharedPointer<SOCKET> clientSocket)
{
    QueryTypes queryTypes = NetworkServer::getMessageFromClient<QueryTypes>(clientSocket);
    ModelTypes modelTypes = NetworkServer::getMessageFromClient<ModelTypes>(clientSocket);
    QString request = NetworkServer::getMessageFromClient(clientSocket);

    QtConcurrent::run([=]()
    {
        QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
        QString result = databaseManager->executeQuery(request);
        sendResult(clientSocket, result, modelTypes, queryTypes);
    });
}

void P_Query::sendResult(QSharedPointer<SOCKET> clientSocket, const QString& result, const ModelTypes modelTypes, const QueryTypes queryTypes)
{
    QMutexLocker locker(NetworkServer::getClientMutex(clientSocket).get());
    PacketTypes packettype = PacketTypes::P_Query;

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modelTypes, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, &queryTypes, sizeof(QueryTypes));
    NetworkServer::sendToClient(clientSocket, result);
}
