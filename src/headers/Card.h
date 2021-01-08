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
public:
    Card(int num);
    static bool isFlush(Card *c1, Card *c2, Card *c3, Card *c4, Card *c5);
    const char* toString();
    int getCardStrength();

    bool operator < (Card* other);
    bool operator <= (Card* other);
    bool operator == (Card* other);
    bool operator >= (Card* other);
    bool operator > (Card* other);
};


#endif //POKER_APP_CARD_H
