#include "p_connectplayertotable.h"

void P_ConnectPlayerToTable::connectPlayerToTable(QSharedPointer<SOCKET> clientSocket)
{
    int tableID;
    recv(*clientSocket, reinterpret_cast<char*>(&tableID), sizeof(int), 0);

    QSharedPointer<Player> player = qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket));
    QSharedPointer<Table> table = Table::getTable(tableID);

    if (table == nullptr)
    {
        P_Notification::sendNotification(clientSocket, TypeMessage::Error, "Uncorrect table");
        return;
    }

    if (!table->canPlayerJoin(player))
    {
        P_Notification::sendNotification(clientSocket, TypeMessage::Error, "You can't join this game");
    }

    table->joinPlayer(player);

    notificationJoining(table, player);

    PacketTypes packettype = PacketTypes::P_ConnectPlayerToTable;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &tableID, sizeof(int));
}

void P_ConnectPlayerToTable::notificationJoining(QSharedPointer<Table> table, QSharedPointer<Player> newPlayer)
{
    for(QSharedPointer<Player> player : table->getPlaers())
    {
        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        P_SendTables::sendTables(clientSocket);

        if(player->getLogin() == newPlayer->getLogin())
            continue;

        int tableID = table->getSettings().ID;
        PacketTypes packettype = PacketTypes::P_ConnectOtherPlayerToTable;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, &tableID, sizeof(int));
    }
}
