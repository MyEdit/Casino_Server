#include "p_notification.h"

const PacketTypes P_Notification::packettype = PacketTypes::P_Notification;

void P_Notification::sendNotification(QSharedPointer<SOCKET> clientSocket, TypeMessage typeMessage, QString message)
{
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &typeMessage, sizeof(TypeMessage));
    NetworkServer::sendToClient(clientSocket, message);
}
