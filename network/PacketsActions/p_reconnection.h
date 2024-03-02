#ifndef P_RECONNECTION_H
#define P_RECONNECTION_H

#include "Network/networkserver.h"

class P_Reconnection
{
private:
    static void reconnectClient(QSharedPointer<SOCKET> clientSocket);

public:
    friend class NetworkServer;
};

#endif // P_RECONNECTION_H
