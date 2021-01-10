//
// Created by Spravca on 2. 1. 2021.
//

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
    this->currentPlayer = -1;
    this->coins = 0;
    this->currentBet = 0;
    this->necessaryBet = 0;
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
    if (this->stage == FIRST_BETS && this->necessaryBet > 0) {
        int bet = this->currentBet;
        if (bet < MIN_BET) bet = MIN_BET;
        return bet;
    }
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

        this->currentPlayer = MAX_PLAYERS;
        for (int i = 0; i < MAX_PLAYERS; i++) {
            Player* p = this->players[i];
            if (p != nullptr) {
                p->receiveCards(cardNums[i * 2], cardNums[i * 2 + 1]);
                if (this->currentPlayer > i) this->currentPlayer = i;
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

void PokerTable::nextPlayer() {
    if (isResumed()) {
        if (this->playersCount == 0) this->currentPlayer = 0;
        else {
            int st = (this->currentPlayer + 1) % MAX_PLAYERS;
            if (st < 0) st += MAX_PLAYERS;
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
    }
}

const char *PokerTable::tableCardsToString() {
    char *str = new char[60];
    bool n3 = this->stage >= GameStage::THREE_CARDS;
    bool n4 = this->stage >= GameStage::FOUR_CARDS;
    bool n5 = this->stage >= GameStage::FIVE_CARDS;

    const char *c1 = n3 ? this->cardsOnTable[0]->toString() : nullptr;
    const char *c2 = n3 ? this->cardsOnTable[1]->toString() : nullptr;
    const char *c3 = n3 ? this->cardsOnTable[2]->toString() : nullptr;
    const char *c4 = n4 ? this->cardsOnTable[3]->toString() : nullptr;
    const char *c5 = n5 ? this->cardsOnTable[4]->toString() : nullptr;

    char *cards = new char[43];
    if (n5) sprintf(cards, "%s | %s | %s | %s | %s", c5, c4, c3, c2, c1);
    else if (n4) sprintf(cards, "%s | %s | %s | %s", c4, c3, c2, c1);
    else if (n3) sprintf(cards, "%s | %s | %s", c3, c2, c1);
    else sprintf(cards, "Prázdny stôl");
    printf(str, "Karty na stole: %s", cards);
    return str;
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
            sprintf(pos, "\nMiesto %d: %18s | %d jednotiek", i + 1, p->getName(), p->countMoney());
            strcat(str, pos);
            if (i == this->currentPlayer) strcat(str, " | na ťahu");
        }
    }
    return str;
}

const char *PokerTable::viewCards(int player) {
    if (player >= 0 && player < MIN_PLAYERS && this->players[player] != nullptr) {
        const char* pCards = this->players[player]->viewCards();
        const char* tCards = this->tableCardsToString();
        char *result = new char[128];
        sprintf(result, "%s\n%s", pCards, tCards);
        return result;
    }
    return "Hráč sa stratil!";
}

Player* PokerTable::getPlayerAt(int index) {
    index %= MAX_PLAYERS;
    if (index < 0) index += MAX_PLAYERS;
    return this->players[index];
}

Player* PokerTable::getCurrentPlayer() {
    if (this->currentPlayer < 0) return nullptr;
    if (this->currentPlayer >= MAX_PLAYERS) return nullptr;
    return this->players[this->currentPlayer];
}

bool PokerTable::makeMove(Move move, char* msg) {
    int len = 128;
    auto* p = this->players[this->currentPlayer < 0 ? 0 : this->currentPlayer >= MAX_PLAYERS ? MAX_PLAYERS - 1 : this->currentPlayer];
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
        strncat(msg, nb || cb ? " (zamietnuté): " : ": ", len - strlen(msg));
        if (nb) strncat(msg, "Ste viazaný povinnou stávkou!", len - strlen(msg));
        else if (cb) strncat(msg, "Dorovnajte stávky alebo sa vzdajte kariet!", len - strlen(msg));
        else strncat(msg, "Bez stavky", len - strlen(msg));
        return !nb && !cb;
    }
    else if (move == FOLD) {
        sprintf(msg, "%6s Zložené karty!", "FOLD");
        p->fold();
        return true;
    }
    else if (move == ALL_IN) {
        int amount = p->allIn();
        this->coins += amount;
        this->currentBet = amount;
        sprintf(msg, "%6s: %d jednotiek!", "ALL_IN", amount);
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
            int payed = p->call(bet);
            this->coins += payed;
            this->necessaryBet--;
            char *money = new char[5];
            sprintf(money, "%d", payed);
            strcat(msg, "Vlozena stavka: -");
            strcat(msg, money);
            return true;
        }
        else sprintf(msg, "Nebola nikým určená stávka!");
        return false;
    }
    else if (move == RAISE) {
        int bet = 2 * getBet();
        if (bet < MIN_BET) bet = MIN_BET;
        bool isAllIn = p->isAllIn(bet / 2);
        bool isAllIn2 = isAllIn && p->isAllIn(bet);
        sprintf(msg, isAllIn ? "%6s (zamietnuté): " : "%6s: ", isAllIn2 ? "ALL IN" : "RAISE");
        if (isAllIn) strncat(msg, "Nedostatok prostriedkov na zvýšenie stávky!", 48);
        else {
            this->currentBet = p->raise(bet);
            this->coins += this->currentBet;
            char *money = new char[5];
            sprintf(money, "%d", this->currentBet);
            strcat(msg, "Vlozena stavka: -");
            strcat(msg, money);
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
    const char *stg = this->stageToString();
    const char *apl = this->activePlayersToString();
    const char *crd = this->tableCardsToString();
    int c = this->coins;
    int cb = this->currentBet;
    char *str = new char[512];
    sprintf(str, "%s\nPeniaze na stole: %d jednotiek\nVýška stávky: %d jednotiek\n%s\n%s", stg, c, cb, apl, crd);
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
    this->necessaryBet = this->stage == FIRST_BETS ? 3 : -1;
    if (this->stage == FIRST_BETS) {
        // povinna stavka vsetkych clenov
        for (int p = 0 ; p < MAX_PLAYERS; p++) {
            Player* player = this->players[p];
            if (player != nullptr) this->coins += player->call(MIN_BET);
        }
    }
    return true;
}

void PokerTable::finishGame() {
    printf("I'm waiting! %d", this->isWaiting());
    while (this->stage != WAITING) {
        printf("I'm waiting in cycle! %d", this->isWaiting());
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

int PokerTable::connectPlayer(const char *name, int socket) {
    if (this->playersCount < MAX_PLAYERS) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (this->players[i] == nullptr) {
                this->players[i] = new Player(name, i, socket);
                this->playersCount++;
                return i;
            }
        }
    }
    return -1;
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
                    strcat(msg2, ": ");
                    strcat(msg2, rating);
                    strcat(msg2, "\n");
                }
                strcat(msg, msg2);
            }
            else {
                sprintf(msg, "Niektorých hráčov sa nepodarilo ohodnotiť! Hra skončila, peniaze prepadlli. Oprav to!");
                this->coins = 0;
            }
            this->stage = WAITING;
        }
    }
    else sprintf(msg, "Hra ešte neskončila alebo nezačala!");
}

int *PokerTable::getSockets() {
    int* sockets = new int[MAX_PLAYERS];
    for (int i = 0; i < MAX_PLAYERS; ++i)
        sockets[i] = this->players[i] != nullptr ? this->players[i]->getSocket() : -1;

    return sockets;
}

int PokerTable::getIdBySocket(int socket) {
    for (int i = 0; i < MAX_PLAYERS; ++i) {
        if (this->players[i] != nullptr && this->players[i]->getSocket() == socket)
            return i;
    }
    return -1;
}

int PokerTable::getPlayersCount() {
    return this->playersCount;
}

int PokerTable::getActivePlayersCount() {
    int n = 0;
    for (int i = 0; i < MAX_PLAYERS; i ++) {
        Player* p = this->players[i];
        if(p != nullptr && p->isPlaying() && !p->isBroke())
            n++;
    }
    return n;
}

