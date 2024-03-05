#include "p_authorization.h"

void P_Authorization::authorizeClient(QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString login = NetworkServer::getMessageFromClient(clientSocket);
    QString password = NetworkServer::getMessageFromClient(clientSocket);
    QString query = QString("SELECT ID_Role FROM StuffUsers WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
    QString responce = databaseManager->executeQuery(query);
    QString fullName;

    if (responce.isEmpty())
    {
        query = QString("SELECT ID_Role FROM Users WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
        responce = databaseManager->executeQuery(query);

        if (responce.isEmpty())
        {
            P_Notification::sendNotification(clientSocket, TypeMessage::Error, "Uncorrect login or password");
            return;
        }

        query = QString("SELECT Name FROM Users WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
        fullName = databaseManager->executeQuery(query);
    }

    query = QString("SELECT Name FROM StuffUsers WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
    fullName = databaseManager->executeQuery(query);

    Message::logInfo("User " + login + " successfully logged");
    PacketTypes packettype = PacketTypes::P_Authorization;
    Roles role = static_cast<Roles>(responce.toInt());

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &role, sizeof(Roles));
    NetworkServer::sendToClient(clientSocket, fullName);
    NetworkServer::addConnect(clientSocket, login);
}
