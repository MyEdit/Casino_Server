#ifndef P_TAKECARD_H
#define P_TAKECARD_H

#include "network/networkserver.h"
#include "Games/Cards/cardRank.h"
#include "Games/Cards/cardSuit.h"

class P_TakeCard
{
public:
    static void takeCard(QSharedPointer<SOCKET> clientSocket);
};

#endif // P_TAKECARD_H
