#include "p_authorization.h"

//Вызывается при получении пакета на авторизацию
void P_Authorization::authorizeClient(QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString login = NetworkServer::getMessageFromClient(clientSocket);
    QString password = NetworkServer::getMessageFromClient(clientSocket);
    QString query = QString("SELECT ID_Role FROM StuffUsers WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
    QString responce = databaseManager->executeQuery(query);

    if (responce == nullptr)
    {
        query = QString("SELECT ID_Role FROM Users WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
        responce = databaseManager->executeQuery(query);

        if (responce == nullptr)
        {
            //TODO: Тут отправляется уведомление, а на клиенте обрабатывается всегда как TypeMessage::Information, нужно дополнительно передавать тип уведомления
            PacketTypes packettype = PacketTypes::P_Notification;
            NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
            NetworkServer::sendToClient(clientSocket, "Uncorrect login or password");
            return;
        }
    }

    Message::logInfo("User " + login + " successfully logged");
    PacketTypes packettype = PacketTypes::P_Authorization;
    Roles role = static_cast<Roles>(responce.toInt());
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &role, sizeof(Roles));
    NetworkServer::addConnect(clientSocket, login);
}
