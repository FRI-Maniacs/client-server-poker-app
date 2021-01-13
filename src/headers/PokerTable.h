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
        WAITING = -1, // nie sú rozdané karty
        FIRST_BETS = 0, // hráči majú rozdané karty
        THREE_CARDS = 3, // na stole je trojica kariet
        FOUR_CARDS = 4, // na stole je štvorica kariet
        FIVE_CARDS = 5, // na stole je 5 kariet
        FINISHED = 6 // hra skoncila, prebieha vyhodnotenie
    };
private:
    Card ** cardsOnTable;
    Player ** players;
    int playersCount;
    Seat currPlayer;
    GameStage stage;
    int coins;
    int necessaryBet;
    int currentBet;
    Seat roundStart;
    static int* generateNumbers(int count);
public:
    PokerTable();
    ~PokerTable();

    bool isWaiting();
    bool isResumed();
    bool isFinished();
    int getBet();

    Player * getPlayerBySocket(int socket);
    // zapis do pola hracov
    Seat connectPlayer(const char* name, int socket);
    bool disconnectPlayer(int pos);

    // priebeh hry
    bool startGame(char * msg);
    bool makeMove(Move, char * msg);
    void nextPlayer();
    int nextStage();
    void finishGame();
    void chooseWinner(char* msg);

    // citanie z pola hracov
    Player* getCurrentPlayer();

    const char* toString();

    const char * viewCards(Seat seat);

    const char *tableCardsToString();

    const char *stageToString();

    const char *activePlayersToString();

    int getPlayersCount() const;

    int getActivePlayersCount();

    bool isActivePlayer(Seat seat = NO_SEAT);

    static void nextSeat(Seat &seat, int step);

    Seat getSeat(int socket);

    bool isOnTurn(int socket);
};


#endif //POKER_APP_POKERTABLE_H
