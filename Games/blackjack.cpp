#include "blackjack.h"

BlackJack::BlackJack()
{
    this->deck = QSharedPointer<Deck>(new Deck());
    //Ticker::addListener(QSharedPointer<QObject>(this), std::bind(&BlackJack::onTick, this));
}

void BlackJack::onTick()
{
    if(!this->isGameRunning())
        return;

    if (this->getPlayers().size() <= 0)
    {
        this->stopGame();
        return;
    }

    if (!this->canStartGame())
    {
        this->onGameFinished();
        return;
    }

    //TODO Добавить таймер
}

void BlackJack::setTable(const int& tableID)
{
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
            Message::logWarn("[" + getName() + "] Client send unknown game packet");
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
    this->playersHands.clear();
}

void BlackJack::giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player)
{
    if (deck->getCountCardInDeck() <= 0)
    {
        P_Notification::sendNotification(clientSocket, TypeMessage::Information, "Deck is empty");
        return;
    }

    Card card = deck->dealCard();
    CardRank rank = card.getRank();
    CardSuit suit = card.getSuit();

    this->playersHands[player].append(card);
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
        NetworkServer::sendToClient(clientSocket, thisPlayer->getLogin());
    }
}

void BlackJack::passTurnToNextPlayer()
{
    QList<QSharedPointer<Player>> players = this->getPlayers();
    int index = this->activePlayer.second + 1;

    if (players.size() - 1 < index)
    {
        this->onGameFinished();
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
    deck = QSharedPointer<Deck>(new Deck());
}

void BlackJack::onGameFinished()
{
    PacketTypes packettype = PacketTypes::P_GamePacket;
    QList<QSharedPointer<Player>> winners = this->getWinners();

    for (QSharedPointer<Player> player : this->playersHands.keys())
    {
        QSharedPointer<SOCKET> playerSocket = NetworkServer::getSocketUser(player);
        GamePackets packetToSend = winners.contains(player) ? GamePackets::P_Win : GamePackets::P_Lose;

        NetworkServer::sendToClient(playerSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(playerSocket, &packetToSend, sizeof(GamePackets));
    }

    this->stopGame();
}

QList<QSharedPointer<Player>> BlackJack::getWinners() const
{
    QList<QSharedPointer<Player>> winners;
    int maxPoints = 0;

    for (auto it = playersHands.begin(); it != playersHands.end(); ++it)
    {
        int playerPoints = 0;

        for (Card card : it.value())
            playerPoints += card.getValue();

        if (playerPoints <= 21)
        {
            if (playerPoints > maxPoints)
            {
                maxPoints = playerPoints;
                winners.clear();
                winners.append(it.key());
            }
            else if (playerPoints == maxPoints)
            {
                winners.append(it.key());
            }
        }
    }

    return winners;
}

QList<QSharedPointer<Player>> BlackJack::getPlayers() const
{
    QSharedPointer<Table> table = Table::getTable(this->tableID);

    if (table == nullptr)
        return QList<QSharedPointer<Player>>();

    return table->getPlayers();
}
