﻿#ifndef PACKET_H
#define PACKET_H

enum class PacketTypes
{
    P_Authorization,
    P_Notification,
    P_DeleteTable,
    P_AddTable,
    P_ConnectPlayerToTable,
    P_SendModel,
    P_SendTables,
    P_QueryWithoutResponce,
    P_Query
};

enum class ModelTypes
{
    Users,
    ActiveTables,
    StuffUsers,
    Banlist
};

enum class QueryTypes
{
    CountEntrites,  //колиество записей
    NumberRow,      //номер строки
    CreateEntry,    //Добавить запись
    UpdateEntry,    //Изменить запись
    DeleteEntry,    //Удалить запись
    Search          //Поиск
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
    Admin = 1,
    TableManager = 2,
    User = 3
};

#endif // PACKET_H
