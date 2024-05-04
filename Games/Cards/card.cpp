#include "card.h"

Card::Card(CardRank cardRank, CardSuit cardSuit)
{
    this->cardRank = cardRank;
    this->cardSuit = cardSuit;
}

CardRank Card::getRank() const
{
    return cardRank;
}

CardSuit Card::getSuit() const
{
    return cardSuit;
}
