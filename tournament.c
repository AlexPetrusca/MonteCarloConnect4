#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#include "tournament.h"
#include "game.h"
#include <stdio.h>

#define DUMP_MOVE_SNAPSHOT(move) \
    printf("%s TO PLAY\n", RED_TO_PLAY ? "RED" : "BLUE"); \
    printf("MOVE PERFORMED: %d\n", move); \
    printf("------------------------------------------------------------------------------------------\n"); \
    DUMP_SNAPSHOT(); \
    true

int determineWinner() {
    if (isWin(bitboard[0])) {
        printf("Red Wins!\n");
        return 1;
    } else if(isWin(bitboard[1])) {
        printf("Blue Wins!\n");
        return -1;
    } else {
        printf("Draw!\n");
        return 0;
    }
}

int playMatch(Player red, Player blue) {
    while (!GAME_OVER) {
        int move;
        if (RED_TO_PLAY) {
            move = red.makeMove();
        } else {
            move = blue.makeMove();
        }
        DUMP_MOVE_SNAPSHOT(move);
    }
    return determineWinner();
}
