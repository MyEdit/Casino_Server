﻿#include "p_connectplayertotable.h"

void P_ConnectPlayerToTable::connectPlayerToTable(QSharedPointer<SOCKET> clientSocket)
{
    int tableID = NetworkServer::getMessageFromClient<int>(clientSocket);
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
        return;
    }

    table->joinPlayer(player);
    notificationJoining(table, player);

    PacketTypes packettype = PacketTypes::P_ConnectPlayerToTable;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &tableID, sizeof(int));
}

void P_ConnectPlayerToTable::playerLeaveTable(QSharedPointer<SOCKET> clientSocket)
{
    int tableID = NetworkServer::getMessageFromClient<int>(clientSocket);
    QSharedPointer<Player> player = qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket));
    QSharedPointer<Table> table = Table::getTable(tableID);

    if (table == nullptr)
    {
        P_Notification::sendNotification(clientSocket, TypeMessage::Error, "Uncorrect table");
        return;
    }

    table->leavePlayer(player);
    notificationJoining(table, player);
}

void P_ConnectPlayerToTable::notificationJoining(QSharedPointer<Table> table, QSharedPointer<Player> newPlayer)
{
    for(QSharedPointer<Player> player : table->getPlayers())
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
