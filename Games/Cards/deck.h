﻿#ifndef DECK_H
#define DECK_H

#include <array>
#include <QVector>
#include "card.h"
#include "Games/Cards/constant.h"

class Deck
{
public:
    using array_type = std::array<Card, deckSize>;
    using index_type = array_type::size_type;

private:
    QVector<Card> deck;
    index_type cardIndex{0};

public:
    Deck();

    void shuffle();
    const Card dealCard();
    int getCountCardInDeck();
};

#endif // DECK_H
