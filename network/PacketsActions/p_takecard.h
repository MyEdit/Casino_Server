#ifndef P_TAKECARD_H
#define P_TAKECARD_H

#include "network/networkserver.h"
#include "Games/cardRank.h"
#include "Games/cardSuit.h"

class P_TakeCard
{
public:
    static void takeCard(QSharedPointer<SOCKET> clientSocket);
    static void notificationTakeCard(QSharedPointer<Table> table, QSharedPointer<Player> currentPlayer);
};

#endif // P_TAKECARD_H
