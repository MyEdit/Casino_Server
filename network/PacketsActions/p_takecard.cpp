#include "p_takecard.h"

void P_TakeCard::takeCard(QSharedPointer<SOCKET> clientSocket)
{
    int tableID = NetworkServer::getMessageFromClient<int>(clientSocket);
    QSharedPointer<Player> player = qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket));
    QSharedPointer<Table> table = Table::getTable(tableID);

    table->getGame()->giveCardToPlayer(clientSocket, player);
}
