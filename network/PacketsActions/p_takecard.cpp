#include "p_takecard.h"

void P_TakeCard::takeCard(QSharedPointer<SOCKET> clientSocket)
{
    CardRank rank = CardRank::RANK_9;
    CardSuit suit = CardSuit::SUIT_DIAMOND;
    int tableID;

    recv(*clientSocket, reinterpret_cast<char*>(&tableID), sizeof(int), 0);

    QSharedPointer<Player> player = qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket));
    QSharedPointer<Table> table = Table::getTable(tableID);

    notificationTakeCard(table, player);

    PacketTypes packettype = PacketTypes::P_TakeCard;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &rank, sizeof(CardRank));
    NetworkServer::sendToClient(clientSocket, &suit, sizeof(CardSuit));
}

void P_TakeCard::notificationTakeCard(QSharedPointer<Table> table, QSharedPointer<Player> currentPlayer)
{
    for(QSharedPointer<Player> player : table->getPlayers())
    {
        if(player->getLogin() == currentPlayer->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_TakeCardAnotherPlayer;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, currentPlayer->getLogin());
    }
}
