#ifndef CARD_H
#define CARD_H

#include "Games/Cards/cardRank.h"
#include "Games/Cards/cardSuit.h"

class Card
{
    CardRank cardRank;
    CardSuit cardSuit;

public:
    Card() {}
    Card(CardRank cardRank, CardSuit cardSuit);

    CardRank getRank() const;
    CardSuit getSuit() const;

    bool operator < (const Card& card) const
    {
        if (cardRank == card.cardRank)
            return cardSuit < card.cardSuit;
        else
            return cardRank < card.cardRank;
    }

    bool operator == (const Card& card) const
    {
        bool rank = cardRank == card.cardRank;
        bool suit = cardSuit == card.cardSuit;

        return rank and suit;
    }
};

#endif // CARD_H
