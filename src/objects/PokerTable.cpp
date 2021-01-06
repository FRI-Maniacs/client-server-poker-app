//
// Created by Spravca on 2. 1. 2021.
//

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../headers/PokerTable.h"
#include "../headers/Card.h"

PokerTable::PokerTable() {
    this->cardsOnTable = static_cast<Card **>(malloc(sizeof(Card *) * 5));
    this->players = static_cast<Player **>(malloc(sizeof(Player *) * MAX_PLAYERS));
    for (int i = 0; i < 5; i++) this->cardsOnTable[i] = nullptr;
    for (int i = 0; i < MAX_PLAYERS; i++) this->players[i] = nullptr;
    this->stage = WAITING;
    this->playersCount = 0;
    this->currentPlayer = -1;
    this->coins = 0;
    this->currentBet = 0;
    this->necessaryBet = 0;
}

bool PokerTable::isWaiting() {
    return this->stage == this->isFinished();
}

bool PokerTable::isResumed() {
    auto s = this->stage;
    return s > WAITING && s < FINISHED;
}

bool PokerTable::isFinished() {
    return this->stage == FINISHED;
}

int PokerTable::getBet() {
    if (!isResumed()) return 0;
    if (this->stage == FIRST_BETS && this->necessaryBet > 0) {
        int bet = this->currentBet;
        if (bet < MIN_BET) bet = MIN_BET;
        return bet;
    }
    return this->currentBet;
}

void PokerTable::generateNumbers(int *array, int count) {
    int cardsLeft = 52;
    int cardsTaken[52] = { 0 };

    for (int pos = 0; pos < count && pos < 52; pos++) {
        int n = ((rand() % 100) * cardsLeft) / 100;
        // najst n-tu volnu kartu
        for (int i = 0; i <= n; i++) if (cardsTaken[i] == 1) n++;
        array[pos] = n;
        cardsTaken[n] ++;
        cardsLeft --;
    }
}

bool PokerTable::startGame(char* str) {
    if (this->isFinished()) sprintf(str, "Čaká sa na vyhodnotenie hry!");
    else if (!this->isWaiting()) sprintf(str, "Hra už beží!");
    else if (playersCount < MIN_PLAYERS) sprintf(str, "Čaká sa na pripojenie hráčov!");
    else {
        int pCount = this->playersCount;
        int cCount = pCount * 2 + 5;
        int* cardNums = new int[cCount];
        generateNumbers(cardNums, cCount);

        for (int i = 0; i < pCount; i++) {
            Player* p = this->players[i];
            if (p != nullptr) p->receiveCards(cardNums[i * 2], cardNums[i * 2 + 1]);
        }

        for (int i = cCount - 5; i < cCount; i++) {
            delete this->cardsOnTable[i];
            this->cardsOnTable[i] = new Card(cardNums[i]);
        }
        delete[] cardNums;
        nextStage();
        sprintf(str, "Hra začala!");
        return true;
    }
    return false;
}

void PokerTable::nextPlayer() {
    if (isResumed()) {
        if (this->playersCount == 0) this->currentPlayer = 0;
        else {
            int st = this->currentPlayer + 1;
            int end = st + MAX_PLAYERS;
            for (; st <= end; st++) {
                int i = st % MAX_PLAYERS;
                if (i == 0) this->nextStage();
                Player * p = this->players[i];
                if (p != nullptr && p->isPlaying() && !p->isBroke()) {
                    this->currentPlayer = i;
                    break;
                }
            }
        }
        if (this->playersCount <= 1) {
            finishGame();
            getCurrentPlayer()->gain(this->coins);
            if (this->playersCount == 0) nextStage();
        }
    }
}

Player* PokerTable::getPlayerAt(int index) {
    index %= MAX_PLAYERS;
    if (index < 0) index += MAX_PLAYERS;
    return this->players[index];
}

Player* PokerTable::getCurrentPlayer() {
    if (this->playersCount == 0) return nullptr;
    int pos = this->currentPlayer;
    for (;pos < pos + MAX_PLAYERS; pos++) {
        auto p = this->players[pos % MAX_PLAYERS];
        if (p != nullptr) return p;
    }
    return nullptr;
}

bool PokerTable::makeMove(Move move, char* msg) {
    Player* p = getCurrentPlayer();
    if (p == nullptr || !p->isPlaying()) {
        sprintf(msg, "Nie je prítomný hráč!");
        return false;
    }
    else if (!this->isResumed()) {
        sprintf(msg, "Hra momentálne nebeží!");
        return false;
    }
    else if (this->playersCount <= 1) {
        finishGame();
        sprintf(msg, "Hra bola náhle ukončená pre nedostatok hráčov!");
        return false;
    }
    else if (move == CHECK) {
        int nb = this->necessaryBet > 0;
        int cb = this->currentBet > 0;
        sprintf(msg, "%6s ", "CHECK");
        sprintf(msg, nb || cb ? " (zamietnuté): " : ": ");
        if (nb) sprintf(msg, "Ste viazaný povinnou stávkou!");
        else if (cb) sprintf(msg, "Dorovnajte stávky alebo sa vzdajte kariet!");
        return !nb && !cb;
    }
    else if (move == FOLD) {
        sprintf(msg, "%6s Zložili ste karty!", "FOLD");
        return true;
    }
    else if (move == ALL_IN) {
        int amount = p->allIn();
        this->coins += amount;
        this->currentBet = amount;
        sprintf(msg, "%6s Vložili ste všetko (%d)!", "ALL_IN", amount);
        return true;
    }
    else if (move == CALL) {
        int bet = getBet();
        if (bet < MIN_BET && this->stage == FIRST_BETS && this->necessaryBet == 0) {
            bet = MIN_BET;
            this->currentBet = MIN_BET;
        }
        int isAllIn = p->isAllIn(bet);
        sprintf(msg, bet < MIN_BET ? "%6s (zamietnuté): " : "%6s: ", isAllIn ? "ALL IN" : "CALL");
        if (bet > 0) {
            this->coins += p->call(bet);
            this->necessaryBet--;
            return true;
        }
        else sprintf(msg, "Nikto pred vami neurčil stávku!");
        return false;
    }
    else if (move == RAISE) {
        int bet = 2 * getBet();
        if (bet < MIN_BET) bet = MIN_BET;
        bool isAllIn = p->isAllIn(bet / 2);
        bool isAllIn2 = isAllIn && p->isAllIn(bet);
        sprintf(msg, isAllIn ? "%6s (zamietnuté): " : "%6s: ", isAllIn2 ? "ALL IN" : "RAISE");
        if (isAllIn) sprintf(msg, "Nemáte dostatok prostriedkov na zvýšenie stávky!");
        else {
            this->currentBet = p->raise(bet);
            this->coins += this->currentBet;
        }
        return !isAllIn;
    }
    return false;
}

PokerTable::~PokerTable() {
    free(this->cardsOnTable);
    free(this->players);
}

const char * PokerTable::toString() {
    if (this->stage == GameStage::WAITING) return "Hra nie je aktívna!";
    if (this->stage == GameStage::FIRST_BETS) return "Žiadne karty ešte nie sú na stole!";
    int size = 100;
    char * str = new char[size];
    strncat(str, "Karty na stole: ", size - strlen(str));
    const char *c1 = this->cardsOnTable[0]->toString();
    const char *c2 = this->cardsOnTable[1]->toString();
    const char *c3 = this->cardsOnTable[2]->toString();
    const char *c4 = this->stage >= GameStage::FOUR_CARDS ? this->cardsOnTable[3]->toString() : nullptr;
    const char *c5 = this->stage >= GameStage::FIVE_CARDS ? this->cardsOnTable[4]->toString() : nullptr;
    if (this->stage == GameStage::THREE_CARDS)
        sprintf(str, "%s | %s | %s", c3, c2, c1);
    else if (this->stage == GameStage::FOUR_CARDS)
        sprintf(str, "%s | %s | %s | %s", c4, c3, c2, c1);
    else if (this->stage == GameStage::FIVE_CARDS)
        sprintf(str, "%s | %s | %s | %s | %s", c5, c4, c3, c2, c1);
    return str;
}

int PokerTable::nextStage() {
    if (this->stage == WAITING && this->playersCount < MIN_PLAYERS) return false;
    switch (this->stage) {
        case WAITING: this->stage = GameStage::FIRST_BETS; break;
        case FIRST_BETS: this->stage = GameStage::THREE_CARDS; break;
        case THREE_CARDS: this->stage = GameStage::FOUR_CARDS; break;
        case FOUR_CARDS: this->stage = GameStage::FIVE_CARDS; break;
        case FIVE_CARDS: this->stage = GameStage::FINISHED; break;
        case FINISHED: this->stage = GameStage::WAITING; break;
    }
    this->currentBet = 0;

    if (this->stage == FIRST_BETS) {
        this->necessaryBet = 3;
        // povinna stavka vsetkych clenov
        for (int p = 0 ; p < MAX_PLAYERS; p++) {
            Player* player = this->players[p];
            if (player != nullptr) player->call(MIN_BET);
        }
    }
    else if (this->stage == THREE_CARDS) {
        this->necessaryBet = 0;
    }
    return true;
}

void PokerTable::finishGame() {
    while (this->stage != WAITING) {
        nextStage();
    }
}

void PokerTable::disconnectPlayer(int pos) {
    int valid = pos >= 0 && pos < MAX_PLAYERS && this->players[pos] != nullptr;
    if (valid) {
        delete this->players[pos];
        this->players[pos] = nullptr;
        this->playersCount--;
    }
    if (valid && this->currentPlayer == pos) nextPlayer();
}

int PokerTable::connectPlayer(char *name) {
    int seat = -1;
    if (this->playersCount < MAX_PLAYERS) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (this->players[i] == nullptr) {
                this->players[seat] = new Player(name, i);
                this->playersCount++;
                seat = i;
                break;
            }
        }
    }
    return seat;
}

void PokerTable::chooseWinner(char *msg) {
    if (this->stage == FINISHED) {
        if (this->playersCount > 1) {
            auto** playersLeft = static_cast<Player**>(malloc(sizeof(Player*)*MAX_PLAYERS));
            auto** hands = static_cast<PokerHand**>(malloc(sizeof(PokerHand*)*MAX_PLAYERS));
            int count = 0;
            for (int i = 0; i < MAX_PLAYERS; i++) {
                Player *p = this->players[i];
                if (p != nullptr && p->isPlaying()) playersLeft[count++] = p;
            }
            // meranie vysledkov
            int invalid = 0;
            for (int i = 0; i < count; i++) {
                hands[i] = PokerHand::getPokerHand(playersLeft[i]->getCards(), this->cardsOnTable, 5);
                invalid = hands[i] == nullptr || invalid;
            }

            // vsetky vysledky uspesne zmerane. Uz len porovnat a odmenit vitaza
            if (!invalid) {
                // zoradenie od vitaza po porazeneho
                for (int p1 = 0; p1 < MAX_PLAYERS; p1++) {
                    for (int p2 = p1 + 1; p2 < MAX_PLAYERS; p2++) {
                        PokerHand *h2 = hands[p1];
                        if (*hands[p1] <= h2) {
                            PokerHand* tmp = hands[p1];
                            hands[p1] = hands[p2];
                            hands[p2] = tmp;
                        }
                    }
                }

                // zistit kolko je vitazov (niekedy maju dvaja rovnake karty)
                int winners = 1;
                for (; winners < count; winners++) {
                    if ((*hands[0]) > hands[winners]) break;
                }

                int reward = this->coins / 3;
                this->coins -= reward;
                sprintf(msg, winners == 1 ? "%d Víťaz:\n" : "%d Víťazi:\n", winners);
                for (int i = 0; i < winners; i++) {
                    playersLeft[i]->gain(reward);
                    strncat(msg, playersLeft[i]->toString(), 500 - strlen(msg));
                    sprintf(msg, "\nVýhra: %d\n", reward);
                }
                sprintf(msg, "\n");
                sprintf(msg, "Kombinácie:\n");
                for (int i = 0; i < count; i++)
                    sprintf(msg, "%18s: %s\n", playersLeft[i]->getName(), hands[i]->toString());
            }
            else
                sprintf(msg, "Niektorých hráčov sa nepodarilo ohodnotiť!");


        }
    }
    else strncat(msg, "Hra ešte neskončila alebo nezačala!", 100);
}
