#include "p_sendtables.h"

const PacketTypes P_SendTables::packettype = PacketTypes::P_SendTables;

void P_SendTables::sendTables(QSharedPointer<SOCKET> clientSocket)
{
    int countTable = Table::getTabels().size();

    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &countTable, sizeof(int));

    for(QSharedPointer<Table> table : Table::getTabels())
    {
        QByteArray jsonData = table->serializeTable();
        int dataSize = jsonData.size();
        NetworkServer::sendToClient(clientSocket, &dataSize, sizeof(int));
        NetworkServer::sendToClient(clientSocket, jsonData.data(), dataSize);
    }
}
