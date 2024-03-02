#include "p_reconnection.h"

void P_Reconnection::reconnectClient(QSharedPointer<SOCKET> clientSocket)
{
    NetworkServer::addConnect(clientSocket, NetworkServer::getMessageFromClient(clientSocket));
}
