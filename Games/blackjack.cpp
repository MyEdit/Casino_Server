#include "blackjack.h"

BlackJack::BlackJack()
{
    this->deck = QSharedPointer<Deck>(new Deck());
}

void BlackJack::startGame()
{
    Game::startGame();

    for (int i = 0; i < 2; ++i)
    {
        for (QSharedPointer<Player> player : this->getPlayers())
        {
            QSharedPointer<SOCKET> playerSocket = NetworkServer::getSocketUser(player);
            giveCardToPlayer(playerSocket, player);
        }
    }
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

QList<QSharedPointer<Player>> BlackJack::getWinners() const
{
    QList<QSharedPointer<Player>> winners;
    int maxPoints = 0;

    for (auto it = playersHands.begin(); it != playersHands.end(); ++it)
    {
        int playerPoints = 0;

        for (Card card : it.value())
            playerPoints += card.getValue();

        if (playerPoints <= maximumScore)
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
