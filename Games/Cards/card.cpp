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

int Card::getValue() const
{
    if (cardRank <= CardRank::RANK_10)
        return (static_cast<int>(cardRank) + 2);

    switch (cardRank)
    {
        case CardRank::RANK_JACK:
        case CardRank::RANK_QUEEN:
        case CardRank::RANK_KING:
            return 10;
        case CardRank::RANK_ACE:
            return 11;
        default:
            return 0;
    }
}
