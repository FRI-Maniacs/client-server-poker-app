#include <cstring>
#include <cstdio>
#include "../headers/Player.h"

#define NAME_LEN 18

Player::Player(const char *name, int p_id, int socket) {
    this->playerId = p_id;
    this->socket = socket;
    char* str = new char[NAME_LEN];
    strncat(str, name, NAME_LEN);
    char *ch = new char[18];
    strncat(ch, name, 18);
    this->name = ch;
    this->coins = 50;
    this->card1 = nullptr;
    this->card2 = nullptr;
}

Player::~Player() {
    delete this->card1;
    delete this->card2;
}

void Player::receiveCards(int c1, int c2) {
    delete this->card1;
    delete this->card2;
    this->card1 = new Card(c1);
    this->card2 = new Card(c2);
}

const char * Player::getName() {
    return this->name;
}

int Player::countMoney() const {
    return this->coins;
}

int Player::call(int bet) {
    return lose(bet);
}

int Player::raise(int bet) {
    return lose(2*bet);
}

void Player::fold() {
    delete this->card1;
    delete this->card2;
    this->card1 = nullptr;
    this->card2 = nullptr;
}

int Player::lose(int n) {
    if (n <= 0) return 0;
    if (this->coins <= n) {
        int give = this->coins;
        this->coins = 0;
        return give;
    }
    this->coins -= n;
    return n;
}

int Player::gain(int n) {
    if (n <= 0) return 0;
    this->coins += n;
    return n;
}

Card** Player::getCards() {
    if (!this->isPlaying()) return nullptr;
    Card** cards = static_cast<Card**>(malloc(sizeof(Card*)*2));
    cards[0] = this->card1;
    cards[1] = this->card2;
    return cards;
}

int Player::isPlaying() {
    return this->card1 != nullptr && this->card2 != nullptr;
}

int Player::isBroke() const {
    return this->coins <= 0;
}

const char* Player::viewCards() {
    if (!this->isPlaying()) return "Bez kariet";
    char * str = new char[14];
    sprintf(str, "%s %s", this->card1->toString(), this->card2->toString());
    return str;
}

bool Player::isAllIn(int bet) const {
    return this->coins <= bet;
}

int Player::allIn() {
    int c = this->coins;
    this->coins = 0;
    return c;
}

int Player::getSocket() const {
    return this->socket;
}


