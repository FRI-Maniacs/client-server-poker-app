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
    for (int i = 0; i < MAX_PLAYERS; i++) {
        this->players[i] = nullptr;
        this->seats[i] = FREE;
    }
    this->stage = STAGE0;
    this->state = WAITING;
    this->playersCount = 0;
    this->currentPlayer = 0;
    this->coins = 0;
    this->currentBet = 0;
    this->necessaryBet = 0;
}

bool PokerTable::generateNumbers(int *array, int count) const {
    if (this->playersCount < MIN_PLAYERS || this->playersCount > MAX_PLAYERS || count <= 0 || count > 52) return false;
    int cardsLeft = 52;
    int cardsTaken[52] = { 0 };

    for (int pos = 0; pos < count; pos++) {
        int n = ((rand() % 100) * cardsLeft) / 100;
        // najst n-tu volnu kartu
        for (int i = 0; i <= n; i++) if (cardsTaken[i] == 1) n++;
        array[pos] = n;
        cardsTaken[n] ++;
        cardsLeft --;
    }
    return true;
}

char* PokerTable::startGame() {
    char * str = new char[48];
    if (this->state == GameState::WAITING && this->stage == GameStage::STAGE0) {
        if (playersCount < MIN_PLAYERS) sprintf(str, "Čaká sa na pripojenie hráčov!");
        else {
            //atomicky zaciatok - hrac sa neodpoji v case ked sa rozdavaju karty, ale az ked sa rozdaju
            int pCount = this->playersCount;

            int* cardNums = new int[pCount];
            generateNumbers(cardNums, pCount);

            for (int i = 0; i < pCount; i++) {
                Player* p = this->players[i];
                if (p != nullptr) p->receiveCards(cardNums[i*2], cardNums[i*2+1]);
            }

            for (int i = pCount * 2; i < pCount * 2 + 5; i++) {
                delete this->cardsOnTable[i];
                this->cardsOnTable[i] = new Card(cardNums[i]);
            }
            delete[] cardNums;

            nextStage();
            sprintf(str, "Hra začala!");
        }
    }
    else if (GameState::FINISHED) sprintf(str, "Ešte sa vyhodnocuje stará hra!");
    else sprintf(str, "Jedna hra už beží!");
    return str;
}

void PokerTable::nextPlayer() {
    if (this->state == RESUMED) {
        if (this->playersCount <= 1) {
            finishGame();
            if (this->playersCount == 1) {
                Player* p = getCurrentPlayer();
                if (p != nullptr) p->gain(this->coins);
            }
            this->coins = 0;
            this->state = WAITING;
        }
        else {
            int st = this->currentPlayer;
            int end = st + MAX_PLAYERS;
            for (; st < end; st++) {
                // malo by sa tu prihliadnúť na to či sa práve hráč neodpája
                int i = st % MAX_PLAYERS;
                Player * p = this->players[i];
                if (i == 0) this->nextStage();
                if (p != nullptr && p->isPlaying() && !p->isBroke()) {
                    this->currentPlayer = i;
                    break;
                }
            }
        }
    }
}

Player* PokerTable::getPlayerAt(int index) {
    index %= MAX_PLAYERS;
    if (index < 0) index += MAX_PLAYERS;
    return this->players[index];
}

Player* PokerTable::getCurrentPlayer() {
    return this->getPlayerAt(this->currentPlayer);
}

bool PokerTable::makeMove(Move move, char* msg) { // msg je hlasenie ktore vidi klient sam
    delete[] msg;
    msg = new char[80];
    // nema zmysel pokracovat ked sa vsetci odpojili. Posledný hráč je víťaz
    if (this->playersCount <= 1) {
        finishGame();
        sprintf(msg, "Hra bola náhle ukončená pre nedostatok hráčov!");
        return true;
    }
    // atomický začiatok - hráč najprv vykoná ťah, čo sľúbil a až potom sa môže odpojiť!
    Player* p = this->getCurrentPlayer();
    int valid = 0;
    switch (move) {
        case CHECK:
            valid = this->currentBet == 0;
            sprintf(msg, valid ? "%6s" : "%6s Stavte sa alebo zložte karty!", "CHECK");
        break;
        case CALL:
            valid = this->currentBet > 0;
            if(!valid) sprintf(msg, "%6s Nie je akú stávku dorovnať!", "CALL");
            else {
                int amount = p->call(this->currentBet);
                this->coins += amount;
                sprintf(msg, "%6s %d jednotiek!", p->isBroke() ? "ALL-IN" : "CALL", amount);
            }
            break;
        case RAISE:
            valid = p->countMoney() <= this->currentBet;
            if (valid) sprintf(msg, "%6s Na zvýšenie stávky nemáte dosť peňazí!", "RAISE");
            else {
                int amount = this->currentBet < MIN_BET ? MIN_BET : this->currentBet * 2;
                this->coins += amount;
                sprintf(msg, "%6s %d jednotiek!", p->isBroke() ? "ALL-IN" : "RAISE", amount);
            }
            break;
        case FOLD: p->fold();
            sprintf(msg, "%6s Zložili ste karty!", "CHECK");
            valid = 1;
            break;
    }
    // atomický koniec
    if (valid) this->nextPlayer();
    return valid;
}

PokerTable::~PokerTable() {
    free(this->cardsOnTable);
}

const char * PokerTable::toString() {
    if (this->stage == GameStage::STAGE0) return "Hra nie je aktívna!";
    if (this->stage == GameStage::STAGE1) return "Žiadne karty ešte nie sú na stole!";
    int size = 100;
    char * str = new char[size];
    strncat(str, "Karty na stole: ", size - strlen(str));
    char str2[size - strlen(str)];
    if (this->stage == GameStage::STAGE2)
        sprintf(str2, "%s | %s | %s",
                this->cardsOnTable[2]->toString(),
                this->cardsOnTable[1]->toString(),
                this->cardsOnTable[0]->toString());
    else if (this->stage == GameStage::STAGE3)
        sprintf(str2, "%s | %s | %s | %s",
                this->cardsOnTable[3]->toString(),
                this->cardsOnTable[2]->toString(),
                this->cardsOnTable[1]->toString(),
                this->cardsOnTable[0]->toString());
    else if (this->stage == GameStage::STAGE4)
        sprintf(str2, "%s | %s | %s | %s | %s",
                this->cardsOnTable[4]->toString(),
                this->cardsOnTable[3]->toString(),
                this->cardsOnTable[2]->toString(),
                this->cardsOnTable[1]->toString(),
                this->cardsOnTable[0]->toString());
    strncat(str, str2, size - strlen(str));
    char* res = new char[strlen(str)];
    strncat(res, str, strlen(str));
    delete[] str;
    return res;
}

int PokerTable::nextStage() {
    //hra zacne minimalne s 2 hracmi
    if (this->stage == STAGE0 && this->playersCount < MIN_PLAYERS) return false;
    switch (this->stage) {
        case STAGE0: this->stage = GameStage::STAGE1; break;
        case STAGE1: this->stage = GameStage::STAGE2; break;
        case STAGE2: this->stage = GameStage::STAGE3; break;
        case STAGE3: this->stage = GameStage::STAGE4; break;
        case STAGE4: this->stage = GameStage::STAGE0; break;
    }
    this->currentBet = 0;

    //hra prave skoncila
    if (this->stage == STAGE0) {
        this->state = FINISHED;
    }

    //v prvej hre maju prvi traja povinnu stavku
    else if (this->stage == STAGE1) {
        this->state = RESUMED;
        this->necessaryBet = 3;

        // hra zacala. Kazdy zucastneny da povinnu stavku na stol

        // atomicky zaciatok - Pre hracov je neskoro sa teraz odpojit. Az po vlozeni stavky!
        for (int p = 0 ; p < MAX_PLAYERS; p++) {
            Player* player = this->players[p];
            if (player != nullptr) player->call(MIN_BET);
        }
        // atomicky koniec - hrac po vlozeni povinnej stavky moze odist
    }
    return true;
}

void PokerTable::finishGame() {
    while (this->stage != STAGE0) {
        nextStage();
    }
}

int PokerTable::findSeat() {
    if (this->playersCount < MAX_PLAYERS) {
        for (int i = 0; i < MAX_PLAYERS; i++) {
            //atomicky zaciatok - citanie i-teho prvku
            if (this->seats[i] == FREE) {
                this->seats[i] = SITTING_DOWN;
            }
            //atomicky koniec
            return i;
        }
    }
    return -1;
}

void PokerTable::disconnectPlayer(int pos) {
    pos = pos < 0 || pos > MAX_PLAYERS ? (pos % MAX_PLAYERS) + (pos < 0 ? MAX_PLAYERS : 0) : pos;
    if (this->players[pos] != nullptr) {
        // atomicky zaciatok
        delete this->players[pos];
        this->players[pos] = nullptr;
        // atomicky koniec
        if (this->currentPlayer == pos) nextPlayer();
    }
}
int PokerTable::connectPlayer(char *name) {
    // je to mozne?
    if (this->playersCount < MAX_PLAYERS) {
        // hlada sa volne miesto
        for (int i = 0; i < MAX_PLAYERS; i++) {
            if (this->seats[i] == FREE) {
                this->seats[i];
            }
            if (this->players[i] == nullptr) {
                this->players[i] = new Player(name, i);
                this->playersCount ++;
                return i;
            }
        }
        //atomicky koniec - koniec zapisu
    }
    return -1;
}

void PokerTable::chooseWinner(char *msg) {
    delete[] msg;
    msg = new char[100];
    if (this->state == FINISHED && this->playersCount > 0) {
        //atomicky zaciatok pripojenie a odpajanie sa nekona
        if (this->playersCount == 1) {

        }


        auto** pl = static_cast<Player**>(malloc(sizeof(this->players)));
        auto** hands = static_cast<PokerHand**>(malloc(sizeof(PokerHand*)*6));
        for (int i = 0; i < MAX_PLAYERS; i++) {
            pl[i] = this->players[i];
            hands[i] = PokerHand::getPokerHand(this->players[i]->getCards(), this->cardsOnTable, 5);
        }
        for (int p1 = 0; p1 < MAX_PLAYERS; p1++) {
            for (int p2 = p1 + 1; p2 < MAX_PLAYERS; p2++) {
                if (pl[p1] == nullptr && pl[p2] == nullptr) continue;
                int swap = 0;
                if (pl[p1] == nullptr) swap++;
                else if ((*hands[p1])>hands[p2]) swap++;
                Player* tmpP = pl[p1];
                PokerHand* tmpH = hands[p1];

            }
        }
    }
    else strncat(msg, "Hra ešte neskončila alebo nezačala!", 100);

}
