#ifndef P_GAMEPACKET_H
#define P_GAMEPACKET_H

#include <network/networkserver.h>
#include <Games/table.h>


class P_GamePacket
{
private:
    static void onGamePacketReceived(QSharedPointer<SOCKET> clientSocket);

public:
    friend class NetworkServer;
};

#endif // P_GAMEPACKET_H
