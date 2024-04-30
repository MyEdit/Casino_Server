#include "p_passmove.h"

void P_PassMove::passMove(QSharedPointer<SOCKET> clientSocket)
{
    int tableID = NetworkServer::getMessageFromClient<int>(clientSocket);
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
    NetworkServer::sendToClient(clientNextSocket, "Ваш");

    for(QSharedPointer<Player> player : players)
    {
        if(player->getLogin() == nextPlayer->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_UpdateGameProcessing;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, nextPlayer->getName());
    }
}
