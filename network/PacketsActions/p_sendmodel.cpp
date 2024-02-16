#include "p_sendmodel.h"

QMap<ModelTypes, QString> P_SendModel::tableNames;

//получает запрашиваемый тип модели
void P_SendModel::getTypeModel(QSharedPointer<SOCKET> clientSocket)
{   
    if (tableNames.size() == 0)
        initMapTableNames();

    ModelTypes modeltype;
    ModelLoadingType modelLoadingType;
    int offset;

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());

    recv(*clientSocket, reinterpret_cast<char*>(&modelLoadingType), sizeof(ModelLoadingType), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&modeltype), sizeof(ModelTypes), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&offset), sizeof(int), 0);

    sendModel(clientSocket, databaseManager->getModel(tableNames[modeltype], offset), modeltype, modelLoadingType);
}

//отправляет запрошенный тип модели
void P_SendModel::sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlTableModel> model, ModelTypes modeltype, ModelLoadingType modelLoadingType)
{
    PacketTypes packettype = PacketTypes::P_SendModel;
    QByteArray jsonData = Serializer::serializeDataModel(model);
    int dataSize = jsonData.size();

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modeltype, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, &modelLoadingType, sizeof(ModelLoadingType));
    NetworkServer::sendToClient(clientSocket, &dataSize, sizeof(int));
    NetworkServer::sendToClient(clientSocket, jsonData.data(), dataSize);
}

//инициилизирует мапу ModelTypes -> имя таблицы
void P_SendModel::initMapTableNames()
{
    tableNames.insert(ModelTypes::Users, "Users");
    tableNames.insert(ModelTypes::ActiveTables, "ActiveTables"); //заменил для теста загрузок моделей
}
