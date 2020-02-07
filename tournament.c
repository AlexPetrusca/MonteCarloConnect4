#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"

#include "tournament.h"
#include "game.h"
#include <stdio.h>

#define DUMP_MOVE_SNAPSHOT(move) \
    printf("%s TO PLAY\n", IS_RED_TO_PLAY ? "RED" : "BLUE"); \
    printf("MOVE PERFORMED: %d\n", move); \
    printf("------------------------------------------------------------------------------------------\n"); \
    DUMP_SNAPSHOT(); \
    true

int determineWinner() {
    if (isWin(bitboard[0])) {
        printf("Red Wins!\n");
        return RED_WIN;
    } else if(isWin(bitboard[1])) {
        printf("Blue Wins!\n");
        return BLUE_WIN;
    } else {
        printf("Draw!\n");
        return DRAW;
    }
}

int playMatch(Player red, Player blue) {
    RESET_GAME();
    while (!GAME_OVER) {
        int move;
        if (IS_RED_TO_PLAY) {
            move = red.makeMove();
        } else {
            move = blue.makeMove();
        }
        DUMP_MOVE_SNAPSHOT(move);
    }
    return determineWinner();
}

void playMatches(Player red, Player blue, int count) {
    int num_games = 0;
    int num_red_wins = 0;
    int num_blue_wins = 0;
    for (int i = 0; i < count; i++) {
        int result = playMatch(red, blue);
        num_games++;
        if (result == RED_WIN) num_red_wins++;
        if (result == BLUE_WIN) num_blue_wins++;
    }

    printf("\nRed Wins: %d", num_red_wins);
    printf("\nBlue Wins: %d", num_blue_wins);
    printf("\nDraw: %d\n", num_games - num_red_wins - num_blue_wins);
}

