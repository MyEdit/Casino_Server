#include "game.h"

QMap<QString, QSharedPointer<Game>> Game::games;
QMutex Game::accessGamesMutex;

Game::Game()
{
    Ticker::addListener(QWeakPointer<Func>(pointerOnTick = QSharedPointer<Func>::create(std::bind(&Game::onTick, this))));
}

void Game::registerGame(QSharedPointer<Game> game)
{
    QMutexLocker locker(&accessGamesMutex);
    games.insert(game->getName(), game);
}

QSharedPointer<Game> Game::getGame(QString name)
{
    QMutexLocker locker(&accessGamesMutex);
    QSharedPointer<Game> game = games[name];

    if (game == nullptr)
        return nullptr;

    return game->getInstance();
}

QSharedPointer<Game> Game::deserializeGame(const QByteArray& data)
{
    QDataStream stream(data);
    QString nameGame;
    stream >> nameGame;
    return Game::getGame(nameGame);
}

void Game::onGamePacketReceived(QSharedPointer<SOCKET> clientSocket)
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

void Game::onTick()
{
    if(!this->isGameRunning())
        return;

    if (this->getPlayers().size() <= 0)
    {
        //this->stopGame();
        return;
    }

    if (!this->canStartGame())
    {
        this->onGameFinished();
        return;
    }

    //TODO Добавить таймер
}

//Особые проверки самой игры на коннект игрока
//В зависимости от игры, условия наверное могут разниться (Например если делать поуровневый доступ игрокам и играм)
//В случае если условий нет - возвращаем true
bool Game::canPlayerJoin(QSharedPointer<Player> player)
{
    return player != nullptr; //Всандалил сюда player != nullptr чтобы QT не давал варн за неиспользованный параметр player
}

bool Game::canStartGame()
{
    if (this->getPlayers().size() < this->getMinPlayers())
        return false;

    return true;
}

QList<QSharedPointer<Player>> Game::getPlayers() const
{
    QMutexLocker locker(&accessGamesMutex);
    QSharedPointer<Table> table = Table::getTable(this->tableID);

    if (table == nullptr)
        return QList<QSharedPointer<Player>>();

    return table->getPlayers();
}

void Game::setTable(const int& tableID)
{
    this->tableID = tableID;
}

int Game::getTableID()
{
    return this->tableID;
}

bool Game::isGameRunning()
{
    return gameRunning;
}

void Game::giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player)
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

    GamePackets gamePacket = GamePackets::P_TakeCard;
    NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
    NetworkServer::sendToClient(clientSocket, &gamePacket, sizeof(GamePackets));
    NetworkServer::sendToClient(clientSocket, &rank, sizeof(CardRank));
    NetworkServer::sendToClient(clientSocket, &suit, sizeof(CardSuit));
}

void Game::notifyOthersTakenCard(QSharedPointer<Player> thisPlayer)
{
    PacketTypes packettype = PacketTypes::P_GamePacket;
    GamePackets gamePacket = GamePackets::P_TakeCardAnotherPlayer;
    QList<QSharedPointer<Player>> players = winners.isEmpty() ? this->getPlayers() : this->winners;

    for(QSharedPointer<Player> player : players)
    {
        if(player->getLogin() == thisPlayer->getLogin())
            continue;

        QSharedPointer<SOCKET> clientSocket = NetworkServer::getSocketUser(qSharedPointerCast<User>(player));
        NetworkServer::sendToClient(clientSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(clientSocket, &gamePacket, sizeof(GamePackets));
        NetworkServer::sendToClient(clientSocket, thisPlayer->getLogin());
    }
}

void Game::passTurnToNextPlayer()
{
    QList<QSharedPointer<Player>> players = winners.isEmpty() ? this->getPlayers() : this->winners;
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

void Game::onGameFinished()
{
    QList<QSharedPointer<Player>> winners = this->getWinners();

    if (winners.size() > 1)
    {
        handleMultipleWinners(winners);
        return;
    }

    for (QSharedPointer<Player> player : this->playersHands.keys())
    {
        QSharedPointer<SOCKET> playerSocket = NetworkServer::getSocketUser(player);
        QSharedPointer<Table> table = Table::getTable(tableID);
        GamePackets packetToSend = winners.contains(player) ? GamePackets::P_Win : GamePackets::P_Lose;

        packetToSend == GamePackets::P_Lose  ? changingBalanceWhenLos(player, table) : changingBalanceWhenWin(player, table);

        NetworkServer::sendToClient(playerSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(playerSocket, &packetToSend, sizeof(GamePackets));
    }

    this->stopGame();
}

void Game::handleMultipleWinners(QList<QSharedPointer<Player>> winners)
{
    this->winners = winners;
    for (QSharedPointer<Player> player : this->playersHands.keys())
    {
        QSharedPointer<SOCKET> playerSocket = NetworkServer::getSocketUser(player);
        QSharedPointer<Table> table = Table::getTable(tableID);
        GamePackets packetToSend = winners.contains(player) ? GamePackets::P_Win : GamePackets::P_Lose;

        if(packetToSend == GamePackets::P_Lose)
            changingBalanceWhenLos(player, table);

        NetworkServer::sendToClient(playerSocket, &packettype, sizeof(PacketTypes));
        NetworkServer::sendToClient(playerSocket, &packetToSend, sizeof(GamePackets));

        if (packetToSend != GamePackets::P_Lose)
            P_Notification::sendNotification(playerSocket, TypeMessage::Information, "У вас ничья, объявлен еще один раунд");
    }

    this->startGame();
}

void Game::changingBalanceWhenWin(QSharedPointer<Player> player, QSharedPointer<Table> table)
{
    double winning = table->getSettings().minBet * (table->getPlayers().size() - 1);
    double commissionCasino = winning * 0.3;
    winning -= commissionCasino;

    double newBalance = player->getBalance() + winning;

    player->setBalance(newBalance);
    creditingProfitsCasino(commissionCasino);
}

void Game::changingBalanceWhenLos(QSharedPointer<Player> player, QSharedPointer<Table> table)
{
    int newBalance = player->getBalance() - table->getSettings().minBet;
    player->setBalance(newBalance);
}

void Game::creditingProfitsCasino(double commissionCasino)
{
    QSharedPointer<DatabaseManager> databaseManager(new DatabaseManager());

    QString data = QDate::currentDate().toString();
    QString summa = QString::number(commissionCasino);
    QString tableNum = QString::number(tableID);
    QString request = "INSERT INTO Profit (Data, Summa, TableNum) VALUES ('" + data + "', '" + summa + "', '" + tableNum + "');";

    QList<QSharedPointer<SOCKET>> adminSockets = NetworkServer::getAdminSockets();
    for(QSharedPointer<SOCKET> adminSocket : adminSockets)
    {
        databaseManager->executeQueryWithoutResponce(request);

        PacketTypes packetTypes = PacketTypes::P_Update;
        ModelTypes modelTypes = ModelTypes::Profit;

        NetworkServer::sendToClient(adminSocket, &packetTypes, sizeof(PacketTypes));
        NetworkServer::sendToClient(adminSocket, &modelTypes, sizeof(ModelTypes));
    }
}

void Game::startGame()
{
    this->gameRunning = true;
    this->activePlayer = {nullptr, -1};
    this->playersHands.clear();
    this->passTurnToNextPlayer();
}

void Game::stopGame()
{
    this->gameRunning = false;
    this->resetDeck();
    this->playersHands.clear();
    this->winners.clear();
}

void Game::resetDeck()
{
    deck = QSharedPointer<Deck>(new Deck());
}
