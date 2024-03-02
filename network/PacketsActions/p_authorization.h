#ifndef P_AUTHORIZATION_H
#define P_AUTHORIZATION_H

#include "Network/networkserver.h"
#include "Network/PacketsActions/p_notification.h"
#include "Database/databasemanager.h"

class P_Authorization
{
private:
    static void authorizeClient(QSharedPointer<SOCKET> clientSocket);

public:
    friend class NetworkServer;
};

#endif // P_AUTHORIZATION_H
