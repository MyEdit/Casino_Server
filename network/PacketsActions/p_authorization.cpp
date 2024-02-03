#include "p_authorization.h"

//���������� ��� ��������� ������ �� �����������
void P_Authorization::authorizeClient(QSharedPointer<SOCKET> clientSocket)
{
    QString login = NetworkServer::getMessageFromClient(clientSocket);
    QString password = NetworkServer::getMessageFromClient(clientSocket);
    QString query = QString("SELECT COUNT(*) FROM users WHERE login = '%1' AND password = '%2'").arg(login).arg(password);
    DatabaseManager manager;

    if (manager.executeQuery(query).toInt() != 1)
    {
        //��� ������� ����������� ������� � ����� � �������� ������/������
        Message::logInfo("Uncorrect login or password");
        return;
    }

    Message::logInfo("Correct login and password (" + login + ", " + password + ")");
    NetworkServer::addConnect(clientSocket, login);
}
