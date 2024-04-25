#ifndef P_NOTIFICATION_H
#define P_NOTIFICATION_H

#include "Network/networkserver.h"

class P_Notification
{
    const static PacketTypes packettype;

public:
    static void sendNotification(QSharedPointer<SOCKET> clientSocket, const TypeMessage typeMessage, const QString& message);
};

#endif // P_NOTIFICATION_H
