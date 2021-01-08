//
// Created by Spravca on 2. 1. 2021.
//

#ifndef POKER_APP_POKERHAND_H
#define POKER_APP_POKERHAND_H

#include "Card.h"
#include "Player.h"
#include "../enums/HandRanking.h"

/**
 * PokerHand slúži na vyhodnocovanie výsledkov kariet a určenie víťaza
 */

class PokerHand {
private:
    static void computeRank(Card **cards, HandRanking &rank, int &strength, char* desc);
    const char* description;
    HandRanking rank;
    int strength;
    PokerHand(HandRanking rank, int strength, const char *desc);
public:
    /**
     * Keď sú karty hráča k dispozícii a na stole sú rozdané aspoň 3 karty, funkcia nájde
     * a ohodnotí najlepšiu možnú kombináciu kariet. Nesplnenie podmienky znamená výsledok nullptr.
     *
     * Výsledky sa zapíšu do novej inštancie, kde už k ďalším zmenám nedochádza.
     *
     * @param player - Hráč, ktorého karty sa priložia ku stolu k ostatným kartam [cards].
     * @param cards - Karty rozdané na stole
     * @param len - Viditeľné množstvo kariet na stole
     * @return PokerHand - vyhodnotenie
     */
    static PokerHand *getPokerHand(Card **playerCards, Card **tableCards, int count);

    /**
     * Vráti reťazec na výpis výsledkov
     * @return
     */
    const char * toString();
    /**
     * Vráti true, ak sú tieto výsledky od výsledkou súpera horšie
     * @param other - výsledky súpera
     * @return
     */
    int operator< (PokerHand *other);
    /**
     * Vráti true, ak sú tieto výsledky od výsledkou súpera zhodné
     * @param other - výsledky súpera
     * @return
     */
    int operator== (PokerHand *other);
    /**
     * Vráti true, ak sú tieto výsledky od výsledkou súpera lepšie
     * @param other - výsledky súpera
     * @return
     */
    int operator> (PokerHand *other);
    /**
     * Vráti true, ak sú tieto výsledky od výsledkou súpera lepšie alebo zhodné
     * @param other - výsledky súpera
     * @return
     */
    int operator>= (PokerHand *other);

    /**
     * Vráti true, ak sú tieto výsledky od výsledkou súpera horšie alebo zhodné
     * @param other - výsledky súpera
     * @return
     */
    int operator<= (PokerHand *other);
};


#endif //POKER_APP_POKERHAND_H
