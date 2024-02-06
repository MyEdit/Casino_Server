#include "p_authorization.h"

//���������� ��� ��������� ������ �� �����������
void P_Authorization::authorizeClient(QSharedPointer<SOCKET> clientSocket)
{
    DatabaseManager manager;
    QString login = NetworkServer::getMessageFromClient(clientSocket);
    QString password = NetworkServer::getMessageFromClient(clientSocket);
    QString query = QString("SELECT COUNT(*) FROM users WHERE login = '%1' AND password = '%2'").arg(login).arg(password);
    QString responce = manager.executeQuery(query);

    if (responce == nullptr || responce.toInt() != 1)
    {
        //��� ������� ����������� ������� � ����� � �������� ������/������
        Message::logInfo("Uncorrect login or password (" + login + ", " + password + ")");
        return;
    }

    Message::logInfo("Correct login and password (" + login + ", " + password + ")");
    PacketTypes packettype = PacketTypes::P_Authorization;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::addConnect(clientSocket, login);
}
