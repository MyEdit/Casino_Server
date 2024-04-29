#include "blackjack.h"

BlackJack::BlackJack()
{

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
    if (this->players.size() < this->getMinPlayers())
        return false;

    return true;
}

bool BlackJack::isGameRunning()
{
    return gameRunning;
}

void BlackJack::startGame()
{
    gameRunning = true;

    QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(players.at(0));
    PacketTypes packettype = PacketTypes::P_StartMove;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, "Ваш");

    for(QSharedPointer<Player> player : players)
    {
        if(player->getLogin() == players.at(0)->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_UpdateGameProcessing;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, players.at(0)->getName());
    }
}

void BlackJack::stopGame()
{
    gameRunning = false;
    this->resetDeck();
}

void BlackJack::giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player)
{
    CardRank rank = CardRank::RANK_9;
    CardSuit suit = CardSuit::SUIT_DIAMOND;

    notifyOthersTakenCard(player);

    PacketTypes packettype = PacketTypes::P_TakeCard;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &rank, sizeof(CardRank));
    NetworkServer::sendToClient(clientSocket, &suit, sizeof(CardSuit));
}

void BlackJack::notifyOthersTakenCard(QSharedPointer<Player> thisPlayer)
{
    for(QSharedPointer<Player> player : this->players)
    {
        if(player->getLogin() == thisPlayer->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(player);
        PacketTypes packettype = PacketTypes::P_TakeCardAnotherPlayer;
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, player->getLogin());
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
