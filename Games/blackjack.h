#ifndef BLACKJACK_H
#define BLACKJACK_H

#include "Games/game.h"

class NetworkServer;
class BlackJack : public Game
{

public:
    BlackJack();
    virtual ~BlackJack() {};

    static constexpr int maximumScore = 21;

    QSharedPointer<Game> getInstance() override;
    QString getName() override;
    int getMinPlayers() override;
    void startGame() override;

private:
    QList<QSharedPointer<Player>> getWinners() const override;
};

#endif // BLACKJACK_H
