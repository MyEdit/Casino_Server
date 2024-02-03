#include "networkserver.h"

QMap<QSharedPointer<SOCKET>, QString> Conections{};
QMutex m_mutex{};

//�������������
bool NetworkServer::init()
{
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1); //������������� ������ ���������� winsock
    if (WSAStartup(DLLVersion, &wsaData) != 0) //�������� ���������� winsock
    {
        Message::logError("WSA library failed to load!");
        return false;
    }
    configuration();
    return true;
}

//��������� ���������� �� ������ �������
void NetworkServer::configuration()
{
    sizeofaddr = sizeof(serverAddress);
    serverAddress.sin_addr.s_addr = inet_addr(ADDRESS);
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_family = AF_INET; //�������� ��������

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    bind(serverSocket, (SOCKADDR*)&serverAddress, sizeofaddr); //���������� ����� ������
}

//������������ ����
void NetworkServer::startListening()
{
    if (!serverAddress.sin_port)
    {
        Message::logError("Port listening failed to start");
        return;
    }

    listen(serverSocket, MAXCONNECTIONS); //������� ���� ��� �������� �������� � ��������
    Message::logInfo("Listening port started successfully!");

    //��������� ��������
    while (Conections.size() != MAXCONNECTIONS) {
        QSharedPointer<SOCKET> clientSocket(new SOCKET);
        *clientSocket = accept(serverSocket, (SOCKADDR*)&serverAddress, &sizeofaddr);

        if (clientSocket == 0)
        {
            Message::logWarn("The client was unable to connect to the server!");
        }
        else
        {
            Message::logInfo("Client [" + getIPAdress(clientSocket) + "] connected!");
            std::thread handler(clientHandler, clientSocket);
            handler.detach();
        }
    }
}

//� ����������� ����� ������� ��������� ��������� �� ������� � �������� packetHandler
void NetworkServer::clientHandler(QSharedPointer<SOCKET> clientSocket)
{
    PacketTypes packettype;
    while(true)
    {
        if (recv(*clientSocket, (char*)&packettype, sizeof(PacketTypes), 0) <= 0)
        {
            onClientDisconnected(clientSocket);
            return;
        }
        packetHandler(packettype, clientSocket);
    }
}

//�� ���� ������������� ����� �����������
void NetworkServer::packetHandler(PacketTypes packettype, QSharedPointer<SOCKET> clientSocket)
{
    switch(packettype)
    {
        case(PacketTypes::P_Authorization):
        {
            P_Authorization::authorizeClient(clientSocket);
            break;
        }
        case(PacketTypes::P_Notification):
        {
            break;
        }
        case(PacketTypes::P_DeleteTable):
        {
            break;
        }
        case(PacketTypes::P_AddTable):
        {
            break;
        }
        case(PacketTypes::P_ConnectPlayerToTable):
        {
            break;
        }
        case(PacketTypes::P_SendModel):
        {
            break;
        }
        case(PacketTypes::P_SendTables):
        {
            break;
        }
        default:
        {
            Message::logWarn("Client send unknown packettype");
            break;
        }
    }
}

//�������� ������ ������������� �������
void NetworkServer::sendToClient(QSharedPointer<SOCKET> client, QString message)
{
    int message_size = message.size();
    send(*client, (char*)&message_size, sizeof(int), 0);
    send(*client, (char*)message.toUtf8().constData(), message_size, 0);
}

//�������� ������ ���� ������������ ��������
void NetworkServer::sendToAllClient(QString message)
{
    QMap<QSharedPointer<SOCKET>, QString>::iterator it;
    for (it = Conections.begin(); it != Conections.end(); ++it)
    {
        QSharedPointer<SOCKET> client = it.key();
        sendToClient(client, message);
    }
}

//���� � ��������� ������ �� ������� � ����������� �������
QString NetworkServer::getMessageFromClient(QSharedPointer<SOCKET> clientSocket) {
    int size;
    recv(*clientSocket, (char*)&size, sizeof(int), 0);
    QByteArray buffer(size, 0);
    recv(*clientSocket, buffer.data(), size, 0);
    return QString(buffer);
}

//���������� IP ����� ������������� �������
QString NetworkServer::getIPAdress(QSharedPointer<SOCKET> client)
{
    sockaddr_in clientAddress;
    int sizeofaddr = sizeof(clientAddress);
    ZeroMemory(&clientAddress, sizeof (clientAddress));
    getsockname(*client, (sockaddr*)&clientAddress, &sizeofaddr);
    return inet_ntoa((in_addr)clientAddress.sin_addr);
}

//���������� ������� ������������� �������
QString NetworkServer::getNickname(QSharedPointer<SOCKET> clientSocket)
{
    return Conections[clientSocket];
}

//���������� ��� ����������� �������
void NetworkServer::onClientDisconnected(QSharedPointer<SOCKET> clientSocket)
{
    QMutexLocker locker(&m_mutex);
    Message::logInfo("Client [" + getIPAdress(clientSocket) + "] disconnected!");
    Conections.remove(clientSocket);
}

//��������� ������� � ���� Conections
void NetworkServer::addConnect(QSharedPointer<SOCKET> clientSocket, QString login)
{
    QMutexLocker locker(&m_mutex);
    Conections.insert(clientSocket, login);
}
