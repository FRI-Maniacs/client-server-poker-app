#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "../headers/PokerTable.h"

PokerTable::PokerTable() {
    this->cardsOnTable = static_cast<Card **>(malloc(sizeof(Card *) * 5));
    this->players = static_cast<Player **>(malloc(sizeof(Player *) * MAX_PLAYERS));
    for (int i = 0; i < 5; i++) this->cardsOnTable[i] = nullptr;
    for (int i = 0; i < MAX_PLAYERS; i++) this->players[i] = nullptr;
    this->stage = WAITING;
    this->playersCount = 0;
    this->currPlayer = NO_SEAT;
    this->coins = 0;
    this->currentBet = 0;
    this->necessaryBet = 0;
    this->roundStart = SEAT1;
}

bool PokerTable::isWaiting() {
    return this->stage == WAITING;
}

bool PokerTable::isResumed() {
    return this->stage != WAITING && this->stage != FINISHED;
}

bool PokerTable::isFinished() {
    return this->stage == FINISHED;
}

int PokerTable::getBet() {
    if (!isResumed()) return 0;
    if (this->necessaryBet > 0 && this->currentBet < MIN_BET) return MIN_BET;
    return this->currentBet;
}

int* PokerTable::generateNumbers(int count) {
    if (count <= 0 || count > 52) return nullptr;
    int *array = new int[count];
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
    return array;
}

bool PokerTable::startGame(char* str) {
    if (this->isFinished()) sprintf(str, "Čaká sa na vyhodnotenie hry!");
    else if (!this->isWaiting()) sprintf(str, "Hra už beží!");
    else if (this->playersCount < MIN_PLAYERS) sprintf(str, "Čaká sa na pripojenie hráčov!");
    else {
        int cCount = this->playersCount * 2 + 5;
        int* cardNums = generateNumbers(cCount);

        this->currPlayer = NO_SEAT;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Player *p = this->players[i];
            if (p != nullptr) {
                p->receiveCards(cardNums[i * 2], cardNums[i * 2 + 1]);
                if (this->currPlayer == NO_SEAT) this->currPlayer = static_cast<Seat>(i);
            }
        }

        int from = cCount - 5;
        for (int i = cCount - 5; i < cCount; i++) {
            delete this->cardsOnTable[i - from];
            this->cardsOnTable[i - from] = new Card(cardNums[i]);
        }
        delete[] cardNums;
        nextStage();
        sprintf(str, "Hra sa úspešne spustila!");
        return true;
    }
    return false;
}

void PokerTable::nextSeat(Seat &seat, int step = 1) {
    if (seat != NO_SEAT) {
        int pos = (((int)seat + step) % MAX_PLAYERS);
        if (pos < 0) pos += MAX_PLAYERS;
        seat = static_cast<Seat>(pos);
    }
}

void PokerTable::nextPlayer() {
    if (isResumed() && this->playersCount > 0 && this->currPlayer != NO_SEAT) {
        Seat s = this->currPlayer;
        for (this->nextSeat(s); s != this->roundStart; this->nextSeat(s)) {
            printf("nextSeat: %d\n", s);
            if (this->isActivePlayer(s)) {
                this->currPlayer = s;
                return;
            }
        }
        this->nextStage();
        for (int i = 0; i < MAX_PLAYERS; i++) {
            s = static_cast<Seat>(i);
            printf("nextSeat2: %d\n", i);
            if (this->isActivePlayer(s)) {
                this->currPlayer = s;
                break;
            }
        }
    }
    else this->currPlayer = NO_SEAT;
}

const char *PokerTable::tableCardsToString() {
    bool n3 = this->stage >= GameStage::THREE_CARDS;
    bool n4 = this->stage >= GameStage::FOUR_CARDS;
    bool n5 = this->stage >= GameStage::FIVE_CARDS;

    const char *c1 = n3 ? this->cardsOnTable[0]->toString() : nullptr;
    const char *c2 = n3 ? this->cardsOnTable[1]->toString() : nullptr;
    const char *c3 = n3 ? this->cardsOnTable[2]->toString() : nullptr;
    const char *c4 = n4 ? this->cardsOnTable[3]->toString() : nullptr;
    const char *c5 = n5 ? this->cardsOnTable[4]->toString() : nullptr;

    char *cards = new char[34];
    if (n5) sprintf(cards, "%s %s %s %s %s", c5, c4, c3, c2, c1);
    else if (n4) sprintf(cards, "%s %s %s %s", c4, c3, c2, c1);
    else if (n3) sprintf(cards, "%s %s %s", c3, c2, c1);
    else sprintf(cards, "Prázdny stôl");
    return cards;
}

const char *PokerTable::stageToString() {
    switch (this->stage) {
        case WAITING : return "Stav: Čakanie";
        case FIRST_BETS : return "Stav: 1. kolo - povinné stávky";
        case THREE_CARDS : return "Stav: 2. kolo - 3 karty na stole";
        case FOUR_CARDS : return "Stav: 3. kolo - 4 karty na stole";
        case FIVE_CARDS : return "Stav: 4. kolo - 5 kariet na stole";
        case FINISHED : return "Stav: Finále - Určovanie víťaza";
    }
}

const char *PokerTable::activePlayersToString() {
    char *str = new char[400];
    sprintf(str, "Pripojení hráči:");
    for (int i = 0; i < MAX_PLAYERS; i++) {
        auto *p = this->players[i];
        if (p != nullptr) {
            char *pos = new char[60];
            sprintf(pos, "\nMiesto %d: %s %d jednotiek", i + 1, p->getName(), p->countMoney());
            strcat(str, pos);
            if (i == this->currPlayer) strcat(str, " | na ťahu");
        }
    }
    return str;
}

Seat PokerTable::getSeat(int socket) {
    for (int i = 0; i < MAX_PLAYERS; i++)
        if (this->players[i] != nullptr && this->players[i]->getSocket() == socket)
            return static_cast<Seat>(i);
    return NO_SEAT;
}

const char *PokerTable::viewCards(Seat seat) {
    if (seat == NO_SEAT || this->players[(int)seat] == nullptr) return "Hráč sa stratil!";
    char *result = new char[128];
    sprintf(result, "%s | %s", this->players[(int)seat]->viewCards(), this->tableCardsToString());
    return result;
}

bool PokerTable::isActivePlayer(Seat seat) {
    if (seat == NO_SEAT) return false;
    Player *p = this->players[(int)seat];
    return p != nullptr && p->isPlaying() && !p->isBroke();
}

Player* PokerTable::getCurrentPlayer() {
    if (this->currPlayer == NO_SEAT) return nullptr;
    return this->players[(int)this->currPlayer];
}

bool PokerTable::makeMove(Move move, char* msg) {
    bzero(msg, strlen(msg));
    auto* p = this->currPlayer == NO_SEAT ? nullptr : this->players[(int)this->currPlayer];
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
        int bet = getBet();
        if (this->necessaryBet > 0) sprintf(msg, "CHECK (zamietnuté): Povinná stávka! [%d jednotiek]", MIN_BET);
        else if (this->currentBet > 0) sprintf(msg, "CHECK (zamietnuté): Vyrovnajte stávku! [%d jednotiek]", this->currentBet);
        else sprintf(msg, "CHECK: Žiadna stávka");
        return bet <= 0;
    }
    else if (move == FOLD) {
        p->fold();
        sprintf(msg, "FOLD: Zloženie kariet!");
        return true;
    }
    else if (move == ALL_IN) {
        int bet = getBet();
        int all = p->allIn();
        if (all > bet) {
            this->currentBet = all;
            this->roundStart = this->currPlayer;
        }
        this->coins += all;
        sprintf(msg, "ALL IN: Vložená stávka: %d jednotiek!", all);
        return true;
    }
    else if (move == CALL) {
        int bet = getBet();
        if (bet >= MIN_BET) {
            int payed = p->call(bet);
            this->coins += payed;
            this->necessaryBet--;
            sprintf(msg,"%s: Vložená stávka: %d jednotiek", p->isBroke() ? "ALL IN" : "CALL", payed);
        }
        else sprintf(msg, "CALL (zamietnuté): Nebola nikým určená stávka!");
        return bet >= MIN_BET;
    }
    else if (move == RAISE) {
        int bet = getBet();
        if (bet < MIN_BET) bet = MIN_BET;
        if (this->necessaryBet > 0) this->necessaryBet = 0;
        bool notEnough = p->isAllIn(bet);
        if (notEnough) sprintf(msg, "RAISE (zamietnuté): Nedostatok prostriedkov na zvýšenie stávky!");
        else {
            bet = (bet < MIN_BET ? MIN_BET : bet) * 2;
            this->currentBet = p->raise(bet);
            this->coins += this->currentBet;
            this->roundStart = this->currPlayer;
            sprintf(msg, "%s: Vložená stávka %d jednotiek", p->isAllIn(bet) ? "ALL IN" : "RAISE", this->currentBet);
        }
        return !notEnough;
    }
    return false;
}

PokerTable::~PokerTable() {
    free(this->cardsOnTable);
    free(this->players);
}

const char * PokerTable::toString() {
    const char *stg = this->stageToString();
    const char *apl = this->activePlayersToString();
    const char *crd = this->tableCardsToString();
    int c = this->coins;
    int cb = this->currentBet;
    char *str = new char[75 + strlen(stg) + strlen(apl) + strlen(crd)];
    sprintf(str, "%s\nPeniaze na stole: %d jednotiek\nVýška stávky: %d jednotiek\n%s\nKarty na stole: %s", stg, c, cb, apl, crd);
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
    this->roundStart = SEAT1;
    this->necessaryBet = this->stage == FIRST_BETS ? 3 : -1;

    // povinne stavky vsetkych clenov
    if (this->stage == FIRST_BETS)
        for (int p = 0 ; p < MAX_PLAYERS; p++)
            if (this->players[p] != nullptr)
                this->coins += this->players[p]->call(MIN_BET);
    return true;
}

void PokerTable::finishGame() {
    if (this->stage != WAITING) this->stage = FINISHED;
}

bool PokerTable::disconnectPlayer(int socket) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (this->players[i] != nullptr && this->players[i]->getSocket() == socket) {
            delete this->players[i];
            this->players[i] = nullptr;
            this->playersCount--;
            return true;
        }
    }
    return false;
}

Seat PokerTable::connectPlayer(const char *name, int socket) {
    if (this->playersCount >= MAX_PLAYERS) return NO_SEAT;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (this->players[i] == nullptr) {
            this->players[i] = new Player(name, i, socket);
            this->playersCount++;
            return static_cast<Seat>(i);
        }
    }
    return NO_SEAT;
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
                        if (*hands[p1] < hands[p2]) {
                            PokerHand *tmpH = hands[p1];
                            Player *tmpP = playersLeft[p1];
                            hands[p1] = hands[p2];
                            playersLeft[p1] = playersLeft[p2];
                            hands[p2] = tmpH;
                            playersLeft[p2] = tmpP;
                        }
                    }
                }

                // zistit kolko je vitazov (niekedy maju dvaja rovnake karty)
                int winners = 1;
                for (; winners < count; winners++) if ((*hands[0]) > hands[winners]) break;

                int reward = this->coins / winners;
                this->coins -= reward * winners;
                sprintf(msg, "Koniec hry\n\n%d %s:\n", winners, winners == 1 ? "Víťaz" : "Víťazi");
                for (int i = 0; i < winners; i++) {
                    playersLeft[i]->gain(reward);
                    strcat(msg, playersLeft[i]->getName());
                    char *win = new char[6];
                    sprintf(win, ": +%d jednotiek\n", reward);
                    strcat(msg, win);
                }
                strcat(msg, "\nKombinácie:\n");
                char *msg2 = new char[256];
                for (int i = 0; i < count; i++) {
                    const char *name = playersLeft[i]->getName();
                    const char *rating = hands[i]->toString();
                    strcat(msg2, name);
                    strcat(msg2, ":\n\t");
                    strcat(msg2, rating);
                    strcat(msg2, "\n");
                }
                strcat(msg, msg2);
            }
            else {
                sprintf(msg, "Niektorých hráčov sa nepodarilo ohodnotiť! Hra skončila, peniaze prepadlli. Oprav to!");
                this->coins = 0;
            }
        }
        this->stage = WAITING;
    }
    else sprintf(msg, "Hra ešte neskončila alebo nezačala!");
}

bool PokerTable::isOnTurn(int socket) {
    return this->getSeat(socket) == this->currPlayer;
}

Player *PokerTable::getPlayerBySocket(int socket) {
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        Player *p = this->players[i];
        if (p != nullptr && p->getSocket() == socket)
            return p;
    }
    return nullptr;
}

int PokerTable::getPlayersCount() const {
    return this->playersCount;
}

int PokerTable::getActivePlayersCount() {
    int n = 0;
    for (int i = 0; i < MAX_PLAYERS; i ++)
        if(this->players[i] != nullptr && this->players[i]
        ->isPlaying() && !this->players[i]->isBroke())
            n++;
    return n;
}

