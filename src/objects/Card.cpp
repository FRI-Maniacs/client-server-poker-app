//
// Created by Spravca on 1. 1. 2021.
//

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
    char* chr = new char[6];
    chr[0] = -30;
    chr[1] = -103;
    chr[2] = this->shape;
    chr[3] = this->face == c10 ? '1' : (char)this->face;
    chr[4] = this->face == c10 ? '0' : ' ';
    chr[5] = '\0';
    this->desc = chr;
}

const char *Card::toString() {
    return this->desc;
}

int Card::getCardStrength() const {
    return this->strength;
}

bool Card::operator<(Card &other) const {
    return this->strength < other.strength;
}

bool Card::operator<=(Card &other) const {
    return this->strength <= other.strength;
}

bool Card::operator==(Card &other) const {
    return this->strength == other.strength;
}

bool Card::operator>=(Card &other) const {
    return this->strength >= other.strength;
}

bool Card::operator>(Card &other) const {
    return this->strength > other.strength;
}

Card::~Card() {
    delete[] desc;
}