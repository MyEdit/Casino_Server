#include "p_passmove.h"

void P_PassMove::passMove(QSharedPointer<SOCKET> clientSocket)
{
    int tableID;

    recv(*clientSocket, reinterpret_cast<char*>(&tableID), sizeof(int), 0);

    QSharedPointer<Player> currentPlayer = qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket));
    QSharedPointer<Table> table = Table::getTable(tableID);

    QList<QSharedPointer<Player>> players = table->getPlayers();
    int currentPlayerIndex = players.indexOf(currentPlayer);
    QSharedPointer<Player> nextPlayer;

    if(currentPlayerIndex != players.size() - 1)
        nextPlayer = players.at(currentPlayerIndex + 1);
    else
        nextPlayer = players.at(0);

    QSharedPointer<SOCKET> clientNextSocket = NetworkServer::getSocketUser(nextPlayer);
    PacketTypes packettype = PacketTypes::P_StartMove;
    NetworkServer::sendToClient(clientNextSocket, &packettype, sizeof(PacketTypes));
}
