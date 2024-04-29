#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Games/game.h"
#include "QList"
#include "QSharedPointer"
#include "Users/player.h"
#include "Games/Cards/card.h"
#include "Games/table.h"

class BlackJack : public Game
{
    QList<QSharedPointer<Player>> players{};
    bool gameRunning {false};
    //QSharedPointer<Deck> deck;

public:
    BlackJack();
    virtual ~BlackJack() {};

    QSharedPointer<Game> getInstance();
    QString getName() override;
    int getMinPlayers() override;
    bool canPlayerJoin(QSharedPointer<Player> player) override;
    bool canStartGame() override;
    bool isGameRunning() override;
    void startGame() override;
    void stopGame() override;
    void giveCardToPlayer(QSharedPointer<SOCKET> clientSocket, QSharedPointer<Player> player) override;

private:
    void notifyOthersTakenCard(QSharedPointer<Player> player);
    void resetDeck();

    //Events
    void onGameFinished();
};

#endif // BLACKJACK_H
