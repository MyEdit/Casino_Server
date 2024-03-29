﻿#include "p_sendmodel.h"


//получает запрашиваемый тип модели
void P_SendModel::getTypeModel(QSharedPointer<SOCKET> clientSocket)
{   
    ModelTypes modeltype;
    ModelLoadingType modelLoadingType;
    int offset;

    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());

    recv(*clientSocket, reinterpret_cast<char*>(&modelLoadingType), sizeof(ModelLoadingType), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&modeltype), sizeof(ModelTypes), 0);
    recv(*clientSocket, reinterpret_cast<char*>(&offset), sizeof(int), 0);
    QString tableName = NetworkServer::getMessageFromClient(clientSocket);
    QString sort = NetworkServer::getMessageFromClient(clientSocket);

    sendModel(clientSocket, databaseManager->getModel(tableName, offset, sort), modeltype, modelLoadingType);
}

//отправляет запрошенный тип модели
void P_SendModel::sendModel(QSharedPointer<SOCKET> clientSocket, QSharedPointer<QSqlQueryModel> model, ModelTypes modeltype, ModelLoadingType modelLoadingType)
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
