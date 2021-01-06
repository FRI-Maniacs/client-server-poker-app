//
// Created by Spravca on 2. 1. 2021.
//

#ifndef POKER_APP_PLAYER_H
#define POKER_APP_PLAYER_H


#pragma once
#include <cstdlib>
#include "Card.h"
#include "PokerHand.h"

class Player {
private:
    int playerId;
    const char * name;
    Card* card1 = nullptr;
    Card* card2 = nullptr;
    int coins;
    int lose(int n);
public:
    Player(const char* name, int p_id);
    int getId();
    ~Player();
    void receiveCards(int c1, int c2);
    int isPlaying();
    int isBroke();
    const char * viewCards();
    int gain(int n);
    Card **getCards();
    int call(int bet);
    int raise(int bet);
    int allIn();
    void fold();
    char* toString();
    int countMoney() const;
    bool isAllIn(int bet);
    const char *getName();
};


#endif //POKER_APP_PLAYER_H
