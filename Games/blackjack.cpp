#include "blackjack.h"

BlackJack::BlackJack(){}

void BlackJack::setTable(const int& tableID)
{
    Message::logInfo("Constuctor");
    this->tableID = tableID;
}

void BlackJack::onGamePacketReceived(QSharedPointer<SOCKET> clientSocket)
{
    GamePackets gamePacket = NetworkServer::getMessageFromClient<GamePackets>(clientSocket);

    switch(gamePacket)
    {
        case(GamePackets::P_TakeCard):
        {
            this->giveCardToPlayer(clientSocket, qSharedPointerCast<Player>(NetworkServer::getUser(clientSocket)));
            break;
        }
        case(GamePackets::P_PassMove):
        {
            this->passTurnToNextPlayer();
            break;
        }
        default:
        {
            //Message::logWarn(new QString("[" + getName() + "] Client send unknown game packet"));
            break;
        }
    }
}

int BlackJack::getTableID()
{
    return this->tableID;
}

QSharedPointer<Game> BlackJack::getInstance()
{
    return QSharedPointer<BlackJack>(new BlackJack());
}

QString BlackJack::getName()
{
    return "BlackJack";
}

int BlackJack::getMinPlayers()
{
    return 2;
}

//Особые проверки самой игры на коннект игрока
//В зависимости от игры, условия наверное могут разниться (Например если делать поуровневый доступ игрокам и играм)
//В случае если условий нет - возвращаем true
bool BlackJack::canPlayerJoin(QSharedPointer<Player> player)
{
    return player != nullptr; //Всандалил сюда player != nullptr чтобы QT не давал варн за неиспользованный параметр player
}

bool BlackJack::canStartGame()
{
    if (this->getPlayers().size() < this->getMinPlayers())
        return false;

    return true;
}

bool BlackJack::isGameRunning()
{
    return gameRunning;
}

void BlackJack::startGame()
{
    this->gameRunning = true;
    this->activePlayer = {nullptr, -1};
    this->passTurnToNextPlayer();
}

void BlackJack::stopGame()
{
    this->gameRunning = false;
    this->resetDeck();
}

void BlackJack::giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player)
{
    CardRank rank = CardRank::RANK_9;
    CardSuit suit = CardSuit::SUIT_DIAMOND;

    this->notifyOthersTakenCard(player);

    PacketTypes packettype = PacketTypes::P_GamePacket;
    GamePackets gamePacket = GamePackets::P_TakeCard;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &gamePacket, sizeof(GamePackets));
    NetworkServer::sendToClient(clientSocket, &rank, sizeof(CardRank));
    NetworkServer::sendToClient(clientSocket, &suit, sizeof(CardSuit));
}

void BlackJack::notifyOthersTakenCard(QSharedPointer<Player> thisPlayer)
{
    PacketTypes packettype = PacketTypes::P_GamePacket;
    GamePackets gamePacket = GamePackets::P_TakeCardAnotherPlayer;

    for(QSharedPointer<Player> player : this->getPlayers())
    {
        if(player->getLogin() == thisPlayer->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(qSharedPointerCast<User>(player));
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, &gamePacket, sizeof(GamePackets));
        NetworkServer::sendToClient(clientSocket, player->getLogin());
    }
}

void BlackJack::passTurnToNextPlayer()
{
    QList<QSharedPointer<Player>> players = this->getPlayers();
    int index = this->activePlayer.second + 1;

    if (players.size() - 1 < index)
    {
        this->stopGame();
        return;
    }

    QSharedPointer<Player> nextPlayer = players.at(index);
    this->activePlayer = {nextPlayer, index};

    QSharedPointer<SOCKET> clientNextSocket = NetworkServer::getSocketUser(qSharedPointerCast<User>(nextPlayer));
    PacketTypes packettype = PacketTypes::P_GamePacket;
    GamePackets gamePacket = GamePackets::P_StartMove;
    NetworkServer::sendToClient(clientNextSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientNextSocket, &gamePacket, sizeof(GamePackets));
    NetworkServer::sendToClient(clientNextSocket, "Ваш");

    for(QSharedPointer<Player> player : players)
    {
        if(player->getLogin() == nextPlayer->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(qSharedPointerCast<User>(player));
        PacketTypes packettype = PacketTypes::P_UpdateGameProcessing;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, nextPlayer->getName());
    }
}

void BlackJack::resetDeck()
{
    //Сбросить колоду в дефолтное состояние
}

void BlackJack::onGameFinished()
{
    this->stopGame();
}

QList<QSharedPointer<Player>> BlackJack::getPlayers()
{
    QSharedPointer<Table> table = Table::getTable(this->tableID);

    if (table == nullptr)
        return QList<QSharedPointer<Player>>();

    return table->getPlayers();
}
