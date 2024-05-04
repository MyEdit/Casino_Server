#include "p_search.h"

void P_Search::getSearchQuary(QSharedPointer<SOCKET> clientSocket)
{
    QString sort = NetworkServer::getMessageFromClient(clientSocket);
    QString table = NetworkServer::getMessageFromClient(clientSocket);
    QString where = NetworkServer::getMessageFromClient(clientSocket);
    QString column = NetworkServer::getMessageFromClient(clientSocket);
    QString searchText = NetworkServer::getMessageFromClient(clientSocket);
    ModelTypes modelTypes = NetworkServer::getMessageFromClient<ModelTypes>(clientSocket);

    SearchManager* manager = new SearchManager(sort, table, where, column, searchText, modelTypes, clientSocket);
    manager->start();
}

void P_Search::sendResult(QSharedPointer<SOCKET> clientSocket, const QString& result, const ModelTypes modelTypes)
{
    PacketTypes packettype = PacketTypes::P_Search;

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modelTypes, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, result);
}

void P_Search::sendNotFound(QSharedPointer<SOCKET> clientSocket)
{
    P_Notification::sendNotification(clientSocket, TypeMessage::Warning, "Данные не найдены");
}
