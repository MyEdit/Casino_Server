#ifndef P_QUERYWITHOUTRESPONCE_H
#define P_QUERYWITHOUTRESPONCE_H

#include "Network/networkserver.h"
#include "Database/databasemanager.h"
#include "Utils/Message.h"

class P_QueryWithoutResponce
{
    static QMap<QueryTypes, QString> typeQueryWithoutResponce;

public:
    static void executeQuary(QSharedPointer<SOCKET> clientSocket);
    static void executionMessage(QSharedPointer<SOCKET> clientSocket, bool result, QueryTypes queryTypes);
    static void initMapTypeQueryWithoutResponce();
};

#endif // P_QUERYWITHOUTRESPONCE_H
