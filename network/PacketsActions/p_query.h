#ifndef P_QUERY_H
#define P_QUERY_H

#include "Network/networkserver.h"
#include "Database/databasemanager.h"
#include "Utils/Message.h"
#include "Utils/serializer.h"

class P_Query
{
    static QMap<QueryTypes, QString> querys;

private:
    static void getResultQuary(QSharedPointer<SOCKET> clientSocket);
    static void sendResult(QSharedPointer<SOCKET> clientSocket, QString result, ModelTypes modelTypes, QueryTypes queryTypes);
    static void initMapQuerys();
    static QString distributor(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);

    static QString countEntrites(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);
    static QString numberRow(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);
    static QString createEntry(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);
    static QString updateEntry(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);
    static QString deleteEntry(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);
    static QString search(QueryTypes queryTypes, ModelTypes modelTypes, QSharedPointer<SOCKET> clientSocket);

public:
    friend class NetworkServer;
};

#endif // P_QUERY_H
