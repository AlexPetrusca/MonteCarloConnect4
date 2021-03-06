#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

// includes
#include "game.h"
#include <stdio.h>
#ifdef production
#include <emscripten/emscripten.h>
#endif


int NUM_COLUMNS = 7;
int NUM_ROWS = 6;

uint64_t bitboard[2];
int heights[7] = { 0, 7, 14, 21, 28, 35, 42 };
uint8_t moves = 0b01111111;
int counter = 0;

const char red_circle[5] = { (uint8_t) 0xF0, (uint8_t) 0x9F, (uint8_t) 0x94, (uint8_t) 0xB4, '\0' };
const char blue_circle[5] = { (uint8_t) 0xF0, (uint8_t) 0x9F, (uint8_t) 0x94, (uint8_t) 0xB5, '\0' };

void printBoard() {
    for (int i = NUM_ROWS - 1; i >= 0; i--) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            if (GET_BITBOARD_BIT(bitboard[0], i, j)) {
                printf("| %s ", red_circle);
            } else if (GET_BITBOARD_BIT(bitboard[1], i, j)) {
                printf("| %s ", blue_circle);
            } else {
                printf("|    ");
            }
        }
        printf("|%*s\n", i + 1, " ");
    }
    printf("------------------------------------\n");
    for (int i = 0; i < NUM_COLUMNS; i++) {
        printf("| %d  ", i);
    }
    printf("|\n");
}

bool isWin(uint64_t player_bitboard) {
    uint32_t directions[] = { 1, 7, 6, 8 };
    uint64_t bb;
    foreach(direction, directions, uint32_t) {
        bb = player_bitboard & (player_bitboard >> *direction);
        if ((bb & (bb >> (2 * *direction))) != 0) {
            return true;
        }
    }
    return false;
}

bool isDraw() {
    return moves == 0;
}

uint8_t getMoves() {
    for (int col = 0; col < NUM_COLUMNS; col++) {
        if (TOP_ROW & BIT_MASK_64(heights[col])) {
            CLEAR_BIT_8(moves, col);
        } else {
            SET_BIT_8(moves, col);
        }
    }
    return moves;
}

void makeMove(unsigned int col) {
    TOGGLE_BIT_64(CURRENT_BITBOARD, heights[col]++);
    getMoves();
    counter++;
}

void unmakeMove(unsigned int col) {
    TOGGLE_BIT_64(PREVIOUS_BITBOARD, --heights[col]);
    getMoves();
    counter--;
}

//#ifdef production
//EMSCRIPTEN_KEEPALIVE
//#endif