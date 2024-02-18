#include "commandnotification.h"

void CommandNotification::execute(std::vector<std::string> args)
{
    if (args.size() < 3)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketByNickname(QString::fromStdString(args[1]));

    if (clientSocket == nullptr)
    {
        Message::logInfo("Player " + QString::fromStdString(args[1]) + " not found");
        return;
    }

    QString message = Command::getTextAfterIndex(args, 2);
    PacketTypes packettype = PacketTypes::P_Notification;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, message);

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
