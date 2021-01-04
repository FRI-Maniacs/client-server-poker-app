//
// Created by Spravca on 1. 1. 2021.
//

#include <cstring>
#include "../headers/Card.h"

bool Card::isFlush(Card* c1, Card* c2, Card* c3, Card* c4, Card* c5) {
    return c1->shape == c2->shape && c2->shape == c3->shape
    && c3->shape == c4->shape && c4->shape == c5->shape;
}

Card::Card(int num) {
    int fixed = num % 52;
    if (fixed < 0) fixed += 52;
    Face faces[] = {c2, c3, c4, c5, c6, c7, c8, c9, c10, J, Q, K, A};
    Suit shapes[] = {DIAMOND, HEART, SPADES, CLUBS};
    this->face = faces[fixed % 13];
    this->shape = shapes[fixed / 13];
    this->strength = (fixed % 13) + 1;
}

const char * Card::toString() {
    int size = this->face == Face::c10 ? 6 : 5;
    char* result = new char[size];
    // obrazok na karte je znak ktory zabera 3 miesta. Prve 2 miesta su vzdy -30 a -90
    result[0] = -30;
    result[1] = -103;
    result[2] = this->shape;
    result[3] = size == 6 ? '1' : (char)this->face;
    result[4] = size == 6 ? '0' : '\0';
    if (size == 6) result[5] = '\0';
    return result;
}

int Card::getCardStrength() {
    return this->strength;
}

bool Card::operator<(Card *other) {
    return this->strength < other->strength;
}

bool Card::operator<=(Card *other) {
    return this->strength <= other->strength;
}

bool Card::operator==(Card *other) {
    return this->strength == other->strength;
}

bool Card::operator>=(Card *other) {
    return this->strength >= other->strength;
}

bool Card::operator>(Card *other) {
    return this->strength > other->strength;
}