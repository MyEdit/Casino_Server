#ifndef P_AUTHORIZATION_H
#define P_AUTHORIZATION_H

#include "Network/networkserver.h"
#include "Network/PacketsActions/p_notification.h"
#include "Database/databasemanager.h"
#include "Users/user.h"
#include "Users/player.h"
#include "Users/stuffuser.h"

class P_Authorization
{
private:
    static void authorizeClient(QSharedPointer<SOCKET> clientSocket);
    static void onPlayerAuth(int ID, QSharedPointer<DatabaseManager> databaseManager, QSharedPointer<SOCKET> clientSocket);
    static void onStuffUserAuth(int ID, QSharedPointer<DatabaseManager> databaseManager, QSharedPointer<SOCKET> clientSocket);
    static void authUser(QSharedPointer<User> user, QSharedPointer<SOCKET> clientSocket);

public:
    friend class NetworkServer;
};

#endif // P_AUTHORIZATION_H
