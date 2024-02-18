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
        //Тут вызвать уведомление пакетом у юзера о неверном пароле/логине
        Message::logInfo("Uncorrect login or password (" + login + ", " + password + ")");
        return;
    }

    Message::logInfo("Correct login and password (" + login + ", " + password + ")");
    PacketTypes packettype = PacketTypes::P_Authorization;
    //Roles role = static_cast<Roles>(responce.toInt()); //TODO: Отсыл роли клиенту, надо реализовать ее получение на клиенте перед открытием формы чтобы роль закинуть в конструктор формы
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    //NetworkServer::sendToClient(clientSocket, &role, sizeof(Roles)); //TODO: Отсыл роли клиенту, надо реализовать ее получение на клиенте перед открытием формы чтобы роль закинуть в конструктор формы
    NetworkServer::addConnect(clientSocket, login);
}
