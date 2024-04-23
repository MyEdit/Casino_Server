#ifndef P_PASSMOVE_H
#define P_PASSMOVE_H

#include "network/networkserver.h"

class P_PassMove
{
public:
    static void passMove(QSharedPointer<SOCKET> clientSocket);
};

#endif // P_PASSMOVE_H
