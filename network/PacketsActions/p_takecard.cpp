#include "p_takecard.h"

void P_TakeCard::takeCard(QSharedPointer<SOCKET> clientSocket)
{
    int tableID;
    recv(*clientSocket, reinterpret_cast<char*>(&tableID), sizeof(int), 0);

    QSharedPointer<Player> player = qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket));
    QSharedPointer<Table> table = Table::getTable(tableID);

    table->getGame()->giveCardToPlayer(clientSocket, player);
}
