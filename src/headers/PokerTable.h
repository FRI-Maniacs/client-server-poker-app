//
// Created by Spravca on 2. 1. 2021.
//

#ifndef POKER_APP_POKERTABLE_H
#define POKER_APP_POKERTABLE_H
#define MAX_PLAYERS 6
#define MIN_PLAYERS 2
#define MIN_BET 2

#include "../enums/HandRanking.h"
#include "../enums/Moves.h"
#include "Card.h"
#include "Player.h"
#include "../enums/Seat.h"

class PokerTable {
    enum GameStage {
        STAGE0 = -1, // nie sú rozdané karty
        STAGE1 = 0, // hráči majú rozdané karty
        STAGE2 = 3, // na stole je trojica kariet
        STAGE3 = 4, // na stole je štvorica kariet
        STAGE4 = 5 // na stole je 5 kariet
    };
    enum GameState {
        WAITING, // Kto sa pripojí, ten sa zúčastní hry
        RESUMED, // hra beží alebo sa rozdáva. Kto sa pripojí, čaká na ďaľšiu hru
        PAUSED, // Hráči nemôžu vykonávať žiadne ťahy. Maximálne sa odpojiť.
        FINISHED // Hra skončila, prebieha vyhodnotenie
    };
private:
    Card ** cardsOnTable;
    Player ** players;
    Seat seats[MAX_PLAYERS];
    int playersCount;
    int currentPlayer;
    GameStage stage;
    GameState state;
    int coins;
    int necessaryBet;
    int currentBet;
    bool generateNumbers(int* array, int count) const;
public:
    PokerTable();
    ~PokerTable();

    // zapis do pola hracov
    int connectPlayer(char* name);
    void disconnectPlayer(int pos);

    // priebeh hry
    char* startGame();
    bool makeMove(Move, char * msg);
    void nextPlayer();
    int nextStage();
    void finishGame();
    void chooseWinner(char* msg);

    // citanie z pola hracov
    Player* getCurrentPlayer();
    Player* getPlayerAt(int index);

    const char* toString();

    int findSeat();
};


#endif //POKER_APP_POKERTABLE_H
