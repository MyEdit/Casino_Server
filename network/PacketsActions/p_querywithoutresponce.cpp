#include "p_querywithoutresponce.h"

QMap<QueryTypes, QString> P_QueryWithoutResponce::typeQueryWithoutResponce;

void P_QueryWithoutResponce::executeQuery(QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QueryTypes queryTypes;

    recv(*clientSocket, reinterpret_cast<char*>(&queryTypes), sizeof(QueryTypes), 0);
    QString request = NetworkServer::getMessageFromClient(clientSocket);

    if(queryTypes == QueryTypes::DeleteEntry)
        executionMessage(clientSocket, databaseManager->executeQueryDeleteWithoutResponce(request), queryTypes);
    else
        executionMessage(clientSocket, databaseManager->executeQueryWithoutResponce(request), queryTypes);
}

void P_QueryWithoutResponce::executionMessage(QSharedPointer<SOCKET> clientSocket, const bool result, const QueryTypes queryTypes)
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
}

void P_QueryWithoutResponce::initMapTypeQueryWithoutResponce()
{
    typeQueryWithoutResponce.insert(QueryTypes::CreateEntry, "Добавление");
    typeQueryWithoutResponce.insert(QueryTypes::UpdateEntry, "Изменение");
    typeQueryWithoutResponce.insert(QueryTypes::DeleteEntry, "Удаление");
}
