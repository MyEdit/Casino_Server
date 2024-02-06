#ifndef P_SENDMODEL_H
#define P_SENDMODEL_H

#include <Network/networkserver.h>
#include <Database/databasemanager.h>

class P_SendModel
{
private:
    static void getTypeModel(QSharedPointer<SOCKET> clientSocket);
    static void sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlTableModel> model, ModelTypes modeltype);
    static void initMapTableNames();

public:
    friend class NetworkServer;
};

#endif // P_SENDMODEL_H
