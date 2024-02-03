#ifndef P_AUTHORIZATION_H
#define P_AUTHORIZATION_H

#include <Network/networkserver.h>
#include <Database/databasemanager.h>

class P_Authorization
{
public:
    friend class NetworkServer;
private:
    static void authorizeClient(QSharedPointer<SOCKET> clientSocket);
};

#endif // P_AUTHORIZATION_H
