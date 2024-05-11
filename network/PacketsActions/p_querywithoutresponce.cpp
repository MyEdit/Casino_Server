#include "p_querywithoutresponce.h"

QMap<QueryTypes, QString> P_QueryWithoutResponce::typeQueryWithoutResponce;
const PacketTypes P_QueryWithoutResponce::packetTypes;

void P_QueryWithoutResponce::executeQuery(QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    ModelTypes modelTypes = NetworkServer::getMessageFromClient<ModelTypes>(clientSocket);
    QueryTypes queryTypes = NetworkServer::getMessageFromClient<QueryTypes>(clientSocket);
    QString request = NetworkServer::getMessageFromClient(clientSocket);

    if(queryTypes == QueryTypes::DeleteEntry)
        executionMessage(clientSocket, databaseManager->executeQueryDeleteWithoutResponce(request), queryTypes, modelTypes);
    else
        executionMessage(clientSocket, databaseManager->executeQueryWithoutResponce(request), queryTypes, modelTypes);
}

void P_QueryWithoutResponce::executionMessage(QSharedPointer<SOCKET> clientSocket, const bool result, const QueryTypes queryTypes, const ModelTypes modelTypes)
{
    if(typeQueryWithoutResponce.isEmpty())
        initMapTypeQueryWithoutResponce();

    QString message;
    TypeMessage typeMessage;

    if(result)
    {
        message = typeQueryWithoutResponce[queryTypes] + " выполено успешно";
        typeMessage = TypeMessage::Information;
    }
    else
    {
        message = typeQueryWithoutResponce[queryTypes] + " не выполено"; //TODO: По хорошему ещё бы сообщать причину, но пока и так соётдет
        typeMessage = TypeMessage::Warning;
    }

    P_Notification::sendNotification(clientSocket, typeMessage, message);
    NetworkServer::sendToClient(clientSocket, &packetTypes, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modelTypes, sizeof(ModelTypes));
}

void P_QueryWithoutResponce::initMapTypeQueryWithoutResponce()
{
    typeQueryWithoutResponce.insert(QueryTypes::CreateEntry, "Добавление");
    typeQueryWithoutResponce.insert(QueryTypes::UpdateEntry, "Изменение");
    typeQueryWithoutResponce.insert(QueryTypes::DeleteEntry, "Удаление");
}
