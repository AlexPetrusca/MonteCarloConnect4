#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

#include "game.h"
#include "players.h"
#include <stdlib.h>
#include <stdio.h>

int makeRandomMove() {
    int move = rand() % NUM_COLUMNS;
    while (!GET_BIT_8(moves, move)) {
        move = (move + 1) % NUM_COLUMNS;
    }
    makeMove(move);
    return move;
}

int getUserMove() {
    printf("Enter Move: ");
    int move = getchar() - 48;
    getchar(); // consume `\n`
    return move;
}

int makeUserMove() {
    int move = getUserMove();
    while (!GET_BIT_8(moves, move)) {
        printf("Invalid move! Try again.\n");
        move = getUserMove();
    }
    makeMove(move);
    return move;
}

Player getUserPlayer() {
    return (Player) { &makeUserMove };
}

Player getRandomPlayer() {
    return (Player) { &makeRandomMove };
}