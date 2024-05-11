﻿#ifndef PACKET_H
#define PACKET_H

enum class PacketTypes
{
    P_Authorization,
    P_Notification,
    P_ConnectPlayerToTable,
    P_SendModel,
    P_SendTables,
    P_QueryWithoutResponce,
    P_Query,
    P_Reconnection,
    P_ConnectOtherPlayerToTable,
    P_PlayerLeaveTable,
    P_UpdateGameProcessing,
    P_GamePacket,
    P_Search,
    P_UpdateBalance,
    P_Update
};

enum class ModelTypes
{
    Users,
    ActiveTables,
    StuffUsers,
    Banlist,
    Credits,
    Payments,
    Profit
};

enum class QueryTypes
{
    CountEntrites,  //колиество записей
    NumberRow,      //номер строки
    CreateEntry,    //Добавить запись
    UpdateEntry,    //Изменить запись
    DeleteEntry,    //Удалить запись
};

enum class ModelLoadingType
{
    Next,
    Central,
    Prev
};

enum class TypeMessage
{
    Information,
    Warning,
    Error
};

enum class Roles
{
    None = -1,
    Admin = 1,
    TableManager = 2,
    User = 3,
};

#endif // PACKET_H
