#include "commandnotification.h"

void CommandNotification::execute(QStringList args)
{
    if (args.size() < 3)
    {
        Command::printUnknownCommandMessage();
        return;
    }

    QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketByNickname(args[1]);

    if (clientSocket == nullptr)
    {
        Message::logInfo("Player " + args[1] + " not found");
        return;
    }

    QString message = Command::getTextAfterIndex(args, 2);
    PacketTypes packettype = PacketTypes::P_Notification;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, message);

    Message::logInfo("Notification successfully sent to player " + args[1]);
}

QString CommandNotification::getCommand()
{
    return "/notification";
}

QString CommandNotification::getHelpInfo()
{
    return "Type /notification <Player> <Message> to send client notifications";
}
