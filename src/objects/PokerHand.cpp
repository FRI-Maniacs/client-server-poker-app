//
// Created by Spravca on 2. 1. 2021.
//
#include <cstring>
#include "../headers/Card.h"
#include "../headers/PokerHand.h"

PokerHand* PokerHand::getPokerHand(Card **playerCards, Card **tableCards, int count) {
    if (count < 3 || count > 5 || playerCards == nullptr || tableCards == nullptr) return nullptr;
    if (playerCards[0] == nullptr && playerCards[1] == nullptr) return nullptr;
    for (int i = 0; i < count; i++) if (tableCards[i] == nullptr) return nullptr;

    Card** comb = static_cast<Card**>(malloc(sizeof(Card*)*5));
    HandRanking rank0 = HIGH_CARD;
    int strength0 = 0;
    char *desc0 = new char[43];
    if (count == 3) {
        comb[0] = tableCards[0];
        comb[1] = tableCards[1];
        comb[2] = tableCards[2];
        comb[3] = playerCards[0];
        comb[4] = playerCards[1];
        PokerHand::computeRank(comb, rank0, strength0, desc0);
    }
    else {
        for (int i = 0; i < count; i++) {
            for (int j = count == 4 ? 4 : i + 1; j < 5; j++) {
                for (int k = 0; k < 5; k++) {
                    comb[k] = i == k ? playerCards[0] : j == k
                            ? playerCards[1] : tableCards[k];
                }
                int tmpS = 0;
                HandRanking tmpH = HIGH_CARD;
                char *tmpC = new char[43];
                tmpC[0] = '\0';
                PokerHand::computeRank(comb, tmpH, tmpS, tmpC);

                int bigger = tmpH > rank0;
                int stronger = tmpH == rank0 && tmpS > strength0;
                if (bigger) rank0 = tmpH;
                if (bigger || stronger) {
                    strength0 = tmpS;
                    desc0[0] = '\0';
                    strncat(desc0, tmpC, strlen(tmpC));
                }
            }
        }
    }
    return new PokerHand(rank0, strength0, desc0);
}

PokerHand::PokerHand(HandRanking rank, int strength, const char *desc) {
    this->strength = strength;
    this->rank = rank;
    this->description = desc;
}

void PokerHand::computeRank(Card ** cards, HandRanking& rank, int& strength, char* desc) {
    // zoradenie kariet pre lahsiu detekciu kombinacii
    for (int i = 0; i < 5; ++i) {
        for (int j = i + 1; j < 5; ++j) {
            if (cards[i] > cards[j]) {
                Card *tmp = cards[i];
                cards[i] = cards[j];
                cards[j] = tmp;
            }
        }
    }
    int lvl2 = 13;
    int lvl3 = 13 * lvl2;
    int lvl4 = 13 * lvl3;
    int lvl5 = 13 * lvl4;

    // sily kariet
    int s1 = cards[0]->getCardStrength();
    int s2 = cards[1]->getCardStrength();
    int s3 = cards[2]->getCardStrength();
    int s4 = cards[3]->getCardStrength();
    int s5 = cards[4]->getCardStrength();

    //dvojice
    int duo1 = s1 == s2;
    int duo2 = s2 == s3;
    int duo3 = s3 == s4;
    int duo4 = s4 == s5;

    // ak sa nenajde ziadna dvojica, moze sa jednat o postupku farebnu zhodu alebo len najvyssiu kartu
    if (!duo1 && !duo2 && !duo3 && !duo4) {
        // farba
        int flush = Card::isFlush(cards[0], cards[1], cards[2], cards[3], cards[4]);
        int straight = s5 - s1 == 4 && s1 + s2 + s3 +s4 +s5 == (s5 * (s5 + 1) - s1 * (s1 - 1)) / 2;
        rank = flush && straight ? STRAIGHT_FLUSH : flush ? FLUSH : straight ? STRAIGHT : HIGH_CARD;
        strength = s5 * lvl5 + s4 * lvl4 + s3*lvl3 + s2 * lvl2 + s1;
    }
    // existuje aspon 1 dvojica
    else {
        int trio1 = duo1 && duo2;
        int trio2 = duo2 && duo3;
        int trio3 = duo3 && duo4;

        int four1 = trio1 && trio2;
        int four2 = trio2 && trio3;

        // stvorica
        if (four1 || four2) {
            rank = FOUR_OF_A_KIND;
            strength = s3 * lvl2 + (four1 ? s5 : s1);
        }
        //fullhouse
        else if (trio1 && duo4 || trio3 && duo1) {
            rank = FULL_HOUSE;
            strength = s3 * lvl2 + (duo1 ? s1 : s5);
        }
        // trojica
        else if (trio1 || trio2 || trio3) {
            rank = THREE_OF_A_KIND;
            strength = s3 * lvl3 + (!trio3 ? s5 : s2) * lvl2 + (!trio1 ? s1 : s4);
        }
        // uz zostavaju iba dvojice
        else {
            int pairs = duo1 + duo2 + duo3 + duo4;
            rank = pairs == 2 ? TWO_PAIRS : ONE_PAIR;
            if (pairs == 2) strength = s4 * lvl3 + s2 * lvl2 + (duo1 && duo3 ? s5 : duo1 ? s3 : s1);
            else if (duo1) strength = s5 * lvl3 + s4 * lvl2 + s3 + s2 * lvl4;
            else if (duo2) strength = s5 * lvl3 + s4 * lvl2 + s3 * lvl4 + s1;
            else if (duo3) strength = s5 * lvl3 + s4 * lvl4 + s2 * lvl2 + s1;
            else strength = s5 * lvl4 + s3 * lvl3 + s2 * lvl2 + s1;
        }
    }

    int len = 100;
    desc[0] = '\0';
    for (int i = 0; i < 5; i++) {
        strcat(desc, cards[i]->toString());
        strcat(desc, i == 5 ? " " : " | ");
    }
    switch (rank) {
        case STRAIGHT_FLUSH: strcat(desc, "Postupka vo farbe"); break;
        case FOUR_OF_A_KIND: strcat(desc, "Štvorica"); break;
        case FULL_HOUSE: strcat(desc, "Full House"); break;
        case FLUSH: strcat(desc, "Farba"); break;
        case STRAIGHT: strcat(desc, "Postupka"); break;
        case THREE_OF_A_KIND: strcat(desc, "Trojica"); break;
        case TWO_PAIRS: strcat(desc, "Dva páry"); break;
        case ONE_PAIR: strcat(desc, "Jeden pár"); break;
        case HIGH_CARD: strcat(desc, "Najvyššia karta"); break;
    }
}

const char * PokerHand::toString() {
    return this->description;
}

int PokerHand::operator<(PokerHand *other) {
    if (other == nullptr) return false;
    return this->rank < other->rank || this->rank == other->rank && this->strength < other->strength;
}

int PokerHand::operator==(PokerHand *other) {
    if (other == nullptr) return false;
    return this->rank == other->rank && this->strength == other->strength;
}

int PokerHand::operator>(PokerHand *other) {
    if (other == nullptr) return true;
    return this->rank > other->rank || this->rank == other->rank && this->strength > other->strength;
}

int PokerHand::operator>=(PokerHand *other) {
    return this > other || this == other;
}

int PokerHand::operator<=(PokerHand *other) {
    return this < other || this == other;
}


