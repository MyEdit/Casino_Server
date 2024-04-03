#include "p_authorization.h"

void P_Authorization::authorizeClient(QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());
    QString login = NetworkServer::getMessageFromClient(clientSocket);
    QString password = NetworkServer::getMessageFromClient(clientSocket);
    QString query = QString("SELECT ID_StuffUser FROM StuffUsers WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
    QString responce = databaseManager->executeQuery(query);

    if (!responce.isEmpty())
    {
        onStuffUserAuth(responce.toInt(), databaseManager, clientSocket);
        return;
    }

    query = QString("SELECT ID_User FROM Users WHERE Login = '%1' AND Password = '%2'").arg(login).arg(password);
    responce = databaseManager->executeQuery(query);

    if (!responce.isEmpty())
    {
        onPlayerAuth(responce.toInt(), databaseManager, clientSocket);
        return;
    }

    P_Notification::sendNotification(clientSocket, TypeMessage::Error, "Uncorrect login or password");
}

void P_Authorization::onPlayerAuth(int ID, QSharedPointer<DatabaseManager> databaseManager, QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<QSqlQuery> responce = databaseManager->executeQueryObject(QString("SELECT * FROM Users WHERE ID_User = '%1'").arg(ID));

    if (responce == nullptr)
        return;

    QSharedPointer<Player> user(new Player
            (
                responce->value(0).toInt(), //ID
                responce->value(1).toString(), //Name
                responce->value(3).toDouble(), //Balance
                responce->value(4).toString(), //Login
                static_cast<Roles>(responce->value(6).toInt()) //Role
             ));

    authUser(user, clientSocket);
}

void P_Authorization::onStuffUserAuth(int ID, QSharedPointer<DatabaseManager> databaseManager, QSharedPointer<SOCKET> clientSocket)
{
    QSharedPointer<QSqlQuery> responce = databaseManager->executeQueryObject(QString("SELECT * FROM StuffUsers WHERE ID_StuffUser = '%1'").arg(ID));

    if (responce == nullptr)
        return;

    QSharedPointer<StuffUser> user(new StuffUser
            (
                responce->value(0).toInt(), //ID
                responce->value(1).toString(), //Name
                responce->value(2).toString(), //Login
                static_cast<Roles>(responce->value(4).toInt()) //Role
             ));

    authUser(user, clientSocket);
}

void P_Authorization::authUser(QSharedPointer<User> user, QSharedPointer<SOCKET> clientSocket)
{
    Message::logInfo("User " + user->getLogin() + " successfully logged");
    PacketTypes packettype = PacketTypes::P_Authorization;
    Roles role = user->getRole();
    QByteArray byteUser = user->serializeUser();
    int sizeByteUser = byteUser.size();

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &role, sizeof(Roles));
    NetworkServer::sendToClient(clientSocket, &sizeByteUser, sizeof(int));
    NetworkServer::sendToClient(clientSocket, byteUser.constData(), sizeByteUser);
    NetworkServer::addConnect(clientSocket, user);
}
