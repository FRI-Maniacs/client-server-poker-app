//
// Created by Spravca on 2. 1. 2021.
//

#ifndef POKER_APP_HANDRANKING_H
#define POKER_APP_HANDRANKING_H
enum HandRanking: int {
    STRAIGHT_FLUSH = 8, // postupka vo farbe
    FOUR_OF_A_KIND = 7, // štvorica
    FULL_HOUSE = 6, // full house
    FLUSH = 5, // farba
    STRAIGHT = 4, // postupka
    THREE_OF_A_KIND = 3, // trojica
    TWO_PAIRS = 2, // 2 páry
    ONE_PAIR = 1, // 1 pár
    HIGH_CARD = 0 // najvyššia karta
};
#endif //POKER_APP_HANDRANKING_H
