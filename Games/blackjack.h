#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "QList"
#include "QSharedPointer"
#include "Users/player.h"

class BlackJack
{
    int minPlayers{2};
    QList<QSharedPointer<Player>> players{};
    //QSharedPointer<Deck> deck;

public:
    BlackJack();

    bool canPlayerJoin(QSharedPointer<Player> player);
    bool canStartGame();
    void startGame();
    void resetDeck(); //Когда игра завершилась, нужно вызвать этот метод и вернуть колоду в дефолтное состояние

    //Events
    void onGameFinished();
    //void onPlayerTakeCard(QSharedPointer<Card> card);
};

#endif // BLACKJACK_H
