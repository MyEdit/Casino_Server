#include "p_sendmodel.h"


//получает запрашиваемый тип модели
void P_SendModel::getTypeModel(QSharedPointer<SOCKET> clientSocket)
{   
    ModelLoadingType modelLoadingType = NetworkServer::getMessageFromClient<ModelLoadingType>(clientSocket);
    ModelTypes modeltype = NetworkServer::getMessageFromClient<ModelTypes>(clientSocket);
    int offset = NetworkServer::getMessageFromClient<int>(clientSocket);
    QString tableName = NetworkServer::getMessageFromClient(clientSocket);
    QString sort = NetworkServer::getMessageFromClient(clientSocket);
    QString where = NetworkServer::getMessageFromClient(clientSocket);

    QtConcurrent::run([=]()
    {
//        QThread::sleep(3);
        QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
        QSharedPointer<QSqlQueryModel> result = databaseManager->getModel(tableName, offset, sort, where);
        sendModel(clientSocket, result, modeltype, modelLoadingType);
    });
}

//отправляет запрошенный тип модели
void P_SendModel::sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlQueryModel> model, const ModelTypes modeltype, const ModelLoadingType modelLoadingType)
{
    QMutexLocker locker(NetworkServer::getClientMutex(clientSocket).get());

    PacketTypes packettype = PacketTypes::P_SendModel;
    QByteArray jsonData = Serializer::serializeDataModel(model);
    int dataSize = jsonData.size();

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &modeltype, sizeof(ModelTypes));
    NetworkServer::sendToClient(clientSocket, &modelLoadingType, sizeof(ModelLoadingType));
    NetworkServer::sendToClient(clientSocket, &dataSize, sizeof(int));
    NetworkServer::sendToClient(clientSocket, jsonData.data(), dataSize);
}
