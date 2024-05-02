#include "p_gamepacket.h"

void P_GamePacket::onGamePacketReceived(QSharedPointer<SOCKET> clientSocket)
{
    int tableID = NetworkServer::getMessageFromClient<int>(clientSocket);
    Table::getTable(tableID)->getGame()->onGamePacketReceived(clientSocket);
}
