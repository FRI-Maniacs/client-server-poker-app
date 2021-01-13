//
// Created by Spravca on 1. 1. 2021.
//

#ifndef POKER_APP_CARD_H
#define POKER_APP_CARD_H

#include "../enums/Face.h"
#include "../enums/Suit.h"

class Card {
private:
    Suit shape;
    Face face;
    int strength;
    const char *desc;
public:
    Card(int num);
    static bool isFlush(Card *c1, Card *c2, Card *c3, Card *c4, Card *c5);
    const char* toString();
    int getCardStrength() const;
    bool operator < (Card& other) const;
    bool operator <= (Card& other) const;
    bool operator == (Card& other) const;
    bool operator >= (Card& other) const;
    bool operator > (Card& other) const;
    ~Card();
};


#endif //POKER_APP_CARD_H
