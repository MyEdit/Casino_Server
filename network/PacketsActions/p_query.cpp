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

    QString request = distributor(queryTypes, modelTypes, clientSocket);

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

void P_Query::initMapQuerys()
{
    querys.insert(QueryTypes::CountEntrites, "SELECT COUNT(*) FROM ");
    querys.insert(QueryTypes::Search, "SELECT numbered_rows.№ FROM (SELECT ROW_NUMBER() OVER (ORDER BY '%2') AS №, * FROM %1) AS numbered_rows WHERE numbered_rows.%2 LIKE '%3");
}

QString P_Query::distributor(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    QString request;

    switch (queryTypes)
    {
    case QueryTypes::CountEntrites:
        request = countEntrites(queryTypes, modelTypes, clientSocket);
        break;
    case QueryTypes::UpdateEntry:
        request = updateEntry(queryTypes, modelTypes, clientSocket);
        break;
    case QueryTypes::DeleteEntry:
        request = deleteEntry(queryTypes, modelTypes, clientSocket);
        break;
    case QueryTypes::CreateEntry:
        request = createEntry(queryTypes, modelTypes, clientSocket);
        break;
    case QueryTypes::NumberRow:
        request = numberRow(queryTypes, modelTypes, clientSocket);
        break;
    case QueryTypes::Search:
        request = search(queryTypes, modelTypes, clientSocket);
        break;
    }

    return request;
}

QString P_Query::countEntrites(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    Q_UNUSED(clientSocket); //пока не нужно
    QString request = querys[queryTypes] + P_SendModel::tableNames[modelTypes];
    return request;
}

QString P_Query::numberRow(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    QString request;

    Q_UNUSED(clientSocket); //пока не нужно
    Q_UNUSED(queryTypes);   //пока не нужно
    Q_UNUSED(modelTypes);   //пока не нужно

    return request;
}

QString P_Query::createEntry(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    QString request;

    Q_UNUSED(clientSocket); //пока не нужно
    Q_UNUSED(queryTypes);   //пока не нужно
    Q_UNUSED(modelTypes);   //пока не нужно

    return request;
}

QString P_Query::updateEntry(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    QString request;

    Q_UNUSED(clientSocket); //пока не нужно
    Q_UNUSED(queryTypes);   //пока не нужно
    Q_UNUSED(modelTypes);   //пока не нужно

    return request;
}

QString P_Query::deleteEntry(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    QString request;

    Q_UNUSED(clientSocket); //пока не нужно
    Q_UNUSED(queryTypes);   //пока не нужно
    Q_UNUSED(modelTypes);   //пока не нужно

    return request;
}

QString P_Query::search(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket)
{
    QString query = NetworkServer::getMessageFromClient(clientSocket);
    QStringList searchList = query.split('/');
    QString request = querys[queryTypes].arg(P_SendModel::tableNames[modelTypes]).arg(searchList[0]).arg(searchList[1]) + searchList[2] + "'";
    return request;
}

