#ifndef CONNECT4_MONTE_CARLO_GAME_H
#define CONNECT4_MONTE_CARLO_GAME_H

#include <stdint.h>
#include <stdbool.h>

// macro definitions
#define foreach(ELEM, ARRAY, TYPE) \
    for (TYPE* ELEM = ARRAY; ELEM < ARRAY + (sizeof(ARRAY) / sizeof(ARRAY[0])); ELEM++)

#define FOREACH_LAMBDA(ARRAY, LAMBDA_FUNC) \
    size_t length = (sizeof(ARRAY) / sizeof(ARRAY[0])); \
    for (int i = 0; i < length; i++) { \
        LAMBDA_FUNC(ARRAY[i]); \
    } \
    true

#define MAP_LAMBDA(ARRAY, LAMBDA_FUNC) \
    size_t length = (sizeof(ARRAY) / sizeof(ARRAY[0])); \
    for (int i = 0; i < length; i++) { \
        ARRAY[i] = LAMBDA_FUNC(ARRAY[i]); \
    } \
    true

#define ONE_8 \
    ((uint8_t) 1)

#define ONE_64 \
    ((uint64_t) 1)

#define BIT_MASK_8(n) \
    (ONE_8 << n)

#define BIT_MASK_64(n) \
    (ONE_64 << n)

#define GET_BIT_8(n, k) \
    ((n & BIT_MASK_8(k)) >> k)

#define GET_BIT_64(n, k) \
    ((n & BIT_MASK_64(k)) >> k)

#define TOGGLE_BIT_8(n, k) \
    n ^= BIT_MASK_8(k)

#define TOGGLE_BIT_64(n, k) \
    n ^= BIT_MASK_64(k)

#define SET_BIT_8(n, k) \
    n |= BIT_MASK_8(k)

#define SET_BIT_64(n, k) \
    n |= BIT_MASK_64(k)

#define CLEAR_BIT_8(n, k) \
    n &= ~BIT_MASK_8(k)

#define CLEAR_BIT_64(n, k) \
    n &= ~(BIT_MASK_64(k))

#define DUMP_BOARD(board) \
    if (true) { \
        char* guide[7]; \
        guide[6] = "  6 13 20 27 34 41 48   55 62         Additional row\n"; \
        guide[5] = "| 5 12 19 26 33 40 47 | 54 61         top row\n"; \
        guide[4] = "| 4 11 18 25 32 39 46 | 53 60\n"; \
        guide[3] = "| 3 10 17 24 31 38 45 | 52 59\n"; \
        guide[2] = "| 2  9 16 23 30 37 44 | 51 58\n"; \
        guide[1] = "| 1  8 15 22 29 36 43 | 50 57\n"; \
        guide[0] = "| 0  7 14 21 28 35 42 | 49 56 63      bottom row\n"; \
        printf("\n"); \
        for (int i = 6; i >= 0; i--) { \
            char* templateString = "| %d  %d  %d  %d  %d  %d  %d |  %d  %d         " ; \
            if (i == 6) { \
                templateString = "  %d  %d  %d  %d  %d  %d  %d    %d  %d         "; \
            } else if (i == 0) { \
                templateString = "| %d  %d  %d  %d  %d  %d  %d |  %d  %d  %d      "; \
            } \
            if (i == 0) { \
                printf(templateString, GET_BIT_64(board, i), GET_BIT_64(board, i + 7), GET_BIT_64(board, i + 14), \
                    GET_BIT_64(board, i + 21), GET_BIT_64(board, i + 28), GET_BIT_64(board, i + 35), GET_BIT_64(board, i + 42), \
                    GET_BIT_64(board, i + 49), GET_BIT_64(board, i + 56), GET_BIT_64(board, i + 63)); \
            } else { \
                printf(templateString, GET_BIT_64(board, i), GET_BIT_64(board, i + 7), GET_BIT_64(board, i + 14), \
                    GET_BIT_64(board, i + 21), GET_BIT_64(board, i + 28), GET_BIT_64(board, i + 35), GET_BIT_64(board, i + 42), \
                    GET_BIT_64(board, i + 49), GET_BIT_64(board, i + 56)); \
            } \
            printf("%s", guide[i]); \
            if (i == 6 || i == 0) { \
                printf("%s", "+---------------------+               +---------------------+\n"); \
            } \
        } \
    } \
    true

#define DUMP_MOVES() \
    printf(" "); \
    for (int i = 0; i < NUM_COLUMNS; i++) { \
        printf(" %s ", GET_BIT_8(moves, i) ? "▲" : " "); \
    } \
    printf("\n "); \
    for (int i = 0; i < NUM_COLUMNS; i++) { \
        printf(" %d ", i); \
    } \
    printf("\n"); \
    true

#define DUMP_HEIGHTS() \
    printf(" "); \
    foreach(height, heights, int) { \
        if (*height / 10 > 0) { \
            printf("%d ", *height); \
        } else { \
            printf(" %d ", *height); \
        } \
    } \
    printf("\n"); \
    true

#define DUMP_SNAPSHOT() \
    DUMP_HEIGHTS(); \
    DUMP_BOARD(bitboard[0]); \
    DUMP_BOARD(bitboard[1]); \
    DUMP_MOVES(); \
    printf("\n"); \
    printBoard(); \
    true

#define TOP_ROW \
    0b1000000100000010000001000000100000010000001000000

#define GET_BITBOARD_BIT(n, row, col) \
    GET_BIT_64(n, row + 7 * col)

#define CURRENT_BITBOARD \
    bitboard[counter & 1]

#define PREVIOUS_BITBOARD \
    bitboard[!(counter & 1)]

#define GAME_OVER \
    (isWin(PREVIOUS_BITBOARD) || isDraw())

#define RED_TO_PLAY \
    (counter & 1)

#define BLUE_TO_PLAY \
    (!(counter & 1))

extern int NUM_COLUMNS;
extern int NUM_ROWS;
extern uint64_t bitboard[2];
extern int heights[7];
extern uint8_t moves;
extern int counter;

int simulateRandomPlayout();
void makeMove(int col);
uint8_t getMoves();
bool isWin(uint64_t player_bitboard);
bool isDraw();
void printBoard();

#endif //CONNECT4_MONTE_CARLO_GAME_H
