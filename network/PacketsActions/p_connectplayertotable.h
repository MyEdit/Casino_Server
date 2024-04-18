#ifndef P_CONNECTPLAYERTOTABLE_H
#define P_CONNECTPLAYERTOTABLE_H

#include "Network/networkserver.h"
#include "Games/table.h"

class P_ConnectPlayerToTable
{
private:
    static void connectPlayerToTable(QSharedPointer<SOCKET> clientSocket);
    static void notificationJoining(QSharedPointer<Table> table, QSharedPointer<Player> newPlayer);

public:
    friend class NetworkServer;
};

#endif // P_CONNECTPLAYERTOTABLE_H
