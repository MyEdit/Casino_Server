﻿#ifndef P_SENDMODEL_H
#define P_SENDMODEL_H

#include "Network/networkserver.h"
#include "Database/databasemanager.h"
#include "Utils/Message.h"
#include "Utils/serializer.h"

class P_SendModel
{
private:
    static void getTypeModel(QSharedPointer<SOCKET> clientSocket);
    static void sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlTableModel> model, ModelTypes modeltype, ModelLoadingType modelLoadingType);

public:
    static QMap<ModelTypes, QString> tableNames;
    static void initMapTableNames();
    friend class NetworkServer;
};

#endif // P_SENDMODEL_H