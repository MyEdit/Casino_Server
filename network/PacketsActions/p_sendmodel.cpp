#include "p_sendmodel.h"

static QMap<ModelTypes, QString> tableNames;

//Получает запрашиваемый тип модели
void P_SendModel::getTypeModel(QSharedPointer<SOCKET> clientSocket)
{
    if (tableNames.size() == 0)
        initMapTableNames();

    QSharedPointer<DatabaseManager> databaseManager;
    ModelTypes modeltype;
    recv(*clientSocket, (char*)&modeltype, sizeof(ModelTypes), 0);
    sendModel(clientSocket, databaseManager->getModel(tableNames[modeltype]), modeltype);
}

//Отправляет запрошенный тип модели
void P_SendModel::sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlTableModel> model, ModelTypes modeltype)
{
    PacketTypes packettype = PacketTypes::P_SendModel;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modeltype, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, &model, sizeof(QSqlTableModel));
}

//Инициилизирует мапу ModelTypes -> ИмяТаблицы
void P_SendModel::initMapTableNames()
{
    tableNames.insert(ModelTypes::Users, "Users");
    tableNames.insert(ModelTypes::Tables, "Tables");
}
