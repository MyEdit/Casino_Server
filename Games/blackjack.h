#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Games/game.h"
#include "QList"
#include "QSharedPointer"
#include "Users/player.h"
#include "Games/Cards/card.h"
#include "Games/table.h"
#include "Games/Cards/deck.h"
#include "network/networkserver.h"

enum class GamePackets
{
    P_TakeCard,
    P_TakeCardAnotherPlayer,
    P_PassMove,
    P_StartMove
};

class NetworkServer;
class BlackJack : public Game
{
    int tableID;
    bool gameRunning {false};
    QPair<QSharedPointer<Player>, int> activePlayer;
    QSharedPointer<Deck> deck;

public:
    BlackJack();
    virtual ~BlackJack() {};

    QSharedPointer<Game> getInstance() override;
    QString getName() override;
    int getMinPlayers() override;
    bool canPlayerJoin(QSharedPointer<Player> player) override;
    bool canStartGame() override;
    bool isGameRunning() override;
    void startGame() override;
    void stopGame() override;
    void giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player) override;
    void setTable(const int& tableID) override;
    void onGamePacketReceived(QSharedPointer<SOCKET> clientSocket) override;
    int getTableID() override;

private:
    void notifyOthersTakenCard(QSharedPointer<Player> player);
    void passTurnToNextPlayer();
    void resetDeck();
    QList<QSharedPointer<Player>> getPlayers();

    //Events
    void onGameFinished();
};

#endif // BLACKJACK_H
