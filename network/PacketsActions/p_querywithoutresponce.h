#ifndef P_QUERYWITHOUTRESPONCE_H
#define P_QUERYWITHOUTRESPONCE_H

#include "network/networkserver.h"
#include "Database/databasemanager.h"
#include "Utils/Message.h"

class P_QueryWithoutResponce
{
    static const PacketTypes packetTypes = PacketTypes::P_Update;
    static QMap<QueryTypes, QString> typeQueryWithoutResponce;

public:
    static void executeQuery(QSharedPointer<SOCKET> clientSocket);
    static void executionMessage(QSharedPointer<SOCKET> clientSocket, const bool result, const QueryTypes queryTypes, const ModelTypes modelTypes);
    static void initMapTypeQueryWithoutResponce();
};

#endif // P_QUERYWITHOUTRESPONCE_H
