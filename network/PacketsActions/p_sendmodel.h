#ifndef P_SENDMODEL_H
#define P_SENDMODEL_H

#include "Network/networkserver.h"
#include "Database/databasemanager.h"
#include "Utils/Message.h"
#include "Utils/serializer.h"
#include <QSharedPointer>

class P_SendModel
{
private:
    static void getTypeModel(QSharedPointer<SOCKET> clientSocket);
    static void sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlQueryModel> model, ModelTypes modeltype, ModelLoadingType modelLoadingType);

public:
    friend class NetworkServer;
};

#endif // P_SENDMODEL_H
