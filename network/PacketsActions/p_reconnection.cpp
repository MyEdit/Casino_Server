#include "p_reconnection.h"

void P_Reconnection::reconnectClient(QSharedPointer<SOCKET> clientSocket)
{
    Roles role = NetworkServer::getMessageFromClient<Roles>(clientSocket);
    int sizeByteUser = NetworkServer::getMessageFromClient<int>(clientSocket);
    QByteArray byteUser = NetworkServer::getMessageFromClient<QByteArray>(clientSocket, sizeByteUser);

    QSharedPointer<User> user;
    if(role == Roles::User)
        user = QSharedPointer<Player>::create(byteUser);
    else
        user = QSharedPointer<StuffUser>::create(byteUser);

    NetworkServer::addConnect(clientSocket, user);
}
