#ifndef P_CONNECTPLAYERTOTABLE_H
#define P_CONNECTPLAYERTOTABLE_H

#include "Network/networkserver.h"

class P_ConnectPlayerToTable
{
private:
    static void connectPlayerToTable(QSharedPointer<SOCKET> clientSocket);
};

public:
    friend class NetworkServer;


#endif // P_CONNECTPLAYERTOTABLE_H
