#include "p_reconnection.h"

void P_Reconnection::reconnectClient(QSharedPointer<SOCKET> clientSocket)
{
    Roles role;
    int sizeByteUser;
    QByteArray byteUser;

    recv(*clientSocket, reinterpret_cast<char*>(&role), sizeof(role), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&sizeByteUser), sizeof(int), 0);
    byteUser.resize(sizeByteUser);
    recv(*clientSocket, byteUser.data(), sizeByteUser, 0);

    QSharedPointer<User> user;
    if(role == Roles::User)
        user = QSharedPointer<Player>::create(byteUser);
    else
        user = QSharedPointer<StuffUser>::create(byteUser);

    NetworkServer::addConnect(clientSocket, user);
}
