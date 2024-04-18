#ifndef P_SENDTABLES_H
#define P_SENDTABLES_H

#include "network/networkserver.h"
#include "Games/table.h"

class P_SendTables
{
public:
    const static PacketTypes packettype;
    static void sendTables(QSharedPointer<SOCKET> clientSocket);
    friend class NetworkServer;
};

#endif // P_SENDTABLES_H
