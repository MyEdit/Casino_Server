﻿#ifndef P_QUERY_H
#define P_QUERY_H

#include "Network/networkserver.h"
#include "Database/databasemanager.h"
#include "Utils/Message.h"
#include "Utils/serializer.h"

class P_Query
{
private:
    static void getResultQuary(QSharedPointer<SOCKET> clientSocket);
    static void sendResult(QSharedPointer<SOCKET> clientSocket, const QString& result, const ModelTypes modelTypes, const QueryTypes queryTypes);

public:
    friend class NetworkServer;
};

#endif // P_QUERY_H
