#include "commandnotification.h"

void CommandNotification::execute(std::vector<std::string> args)
{
    if (args.size() != 3)
    {
        Message::logInfo(Command::getUnknownCommandMessage());
        return;
    }

    QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketByNickname(QString::fromStdString(args[1]));

    if (clientSocket == nullptr)
    {
        Message::logInfo("Player " + QString::fromStdString(args[1]) + " not found");
        return;
    }

    PacketTypes packettype = PacketTypes::P_Notification;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, QString::fromStdString(args[2]));

    Message::logInfo("Notification successfully sent to player " + QString::fromStdString(args[1]));
}

std::string CommandNotification::getCommand()
{
    return "/notification";
}

std::string CommandNotification::getHelpInfo()
{
    return "Type /notification <Player> <Message> to send client notifications";
}
