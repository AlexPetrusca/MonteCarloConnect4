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

#define TAKE_SNAPSHOT() \
    uint64_t bitboard_snapshot[2] = { bitboard[0], bitboard[1] }; \
    int heights_snapshot[7] = { heights[0], heights[1], heights[2], heights[3], heights[4], heights[5], heights[6] }; \
    uint8_t moves_snapshot = moves; \
    int counter_snapshot = counter; \
    true

#define REVERT_TO_SNAPSHOT() \
    bitboard[0] = bitboard_snapshot[0]; \
    bitboard[1] = bitboard_snapshot[1]; \
    heights[0] = heights_snapshot[0]; \
    heights[1] = heights_snapshot[1]; \
    heights[2] = heights_snapshot[2]; \
    heights[3] = heights_snapshot[3]; \
    heights[4] = heights_snapshot[4]; \
    heights[5] = heights_snapshot[5]; \
    heights[6] = heights_snapshot[6]; \
    moves = moves_snapshot; \
    counter = counter_snapshot; \
    true

#define RESET_GAME() \
    bitboard[0] = 0; \
    bitboard[1] = 0; \
    heights[0] = 0; \
    heights[1] = 7; \
    heights[2] = 14; \
    heights[3] = 21; \
    heights[4] = 28; \
    heights[5] = 35; \
    heights[6] = 42; \
    moves = 0b01111111; \
    counter = 0; \
    true

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
        for (int __i = 6; __i >= 0; __i--) { \
            char* templateString = "| %d  %d  %d  %d  %d  %d  %d |  %d  %d         " ; \
            if (__i == 6) { \
                templateString = "  %d  %d  %d  %d  %d  %d  %d    %d  %d         "; \
            } else if (__i == 0) { \
                templateString = "| %d  %d  %d  %d  %d  %d  %d |  %d  %d  %d      "; \
            } \
            if (__i == 0) { \
                printf(templateString, GET_BIT_64(board, __i), GET_BIT_64(board, __i + 7), GET_BIT_64(board, __i + 14), \
                    GET_BIT_64(board, __i + 21), GET_BIT_64(board, __i + 28), GET_BIT_64(board, __i + 35), GET_BIT_64(board, __i + 42), \
                    GET_BIT_64(board, __i + 49), GET_BIT_64(board, __i + 56), GET_BIT_64(board, __i + 63)); \
            } else { \
                printf(templateString, GET_BIT_64(board, __i), GET_BIT_64(board, __i + 7), GET_BIT_64(board, __i + 14), \
                    GET_BIT_64(board, __i + 21), GET_BIT_64(board, __i + 28), GET_BIT_64(board, __i + 35), GET_BIT_64(board, __i + 42), \
                    GET_BIT_64(board, __i + 49), GET_BIT_64(board, __i + 56)); \
            } \
            printf("%s", guide[__i]); \
            if (__i == 6 || __i == 0) { \
                printf("%s", "+---------------------+               +---------------------+\n"); \
            } \
        } \
    } \
    true

#define DUMP_MOVES() \
    printf(" "); \
    for (int __i = 0; __i < NUM_COLUMNS; __i++) { \
        printf(" %s ", GET_BIT_8(moves, __i) ? "▲" : " "); \
    } \
    printf("\n "); \
    for (int __i = 0; __i < NUM_COLUMNS; __i++) { \
        printf(" %d ", __i); \
    } \
    printf("\n"); \
    true

#define DUMP_HEIGHTS() \
    if (true) { \
        printf(" "); \
        foreach(height, heights, int) { \
            if (*height / 10 > 0) { \
                printf("%d ", *height); \
            } else { \
                printf(" %d ", *height); \
            } \
        } \
        printf("\n"); \
    } \
    true

#define DUMP_SNAPSHOT() \
    DUMP_HEIGHTS(); \
    DUMP_BOARD(bitboard[0]); \
    DUMP_BOARD(bitboard[1]); \
    DUMP_MOVES(); \
    printf("\n"); \
    printBoard(); \
    printf("\n"); \
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

#define IS_RED_TO_PLAY \
    (!(counter & 1))

#define IS_BLUE_TO_PLAY \
    (counter & 1)

enum result { RED_WIN, BLUE_WIN, DRAW };

extern int NUM_COLUMNS;
extern int NUM_ROWS;
extern uint64_t bitboard[2];
extern int heights[7];
extern uint8_t moves;
extern int counter;

void makeMove(unsigned int col);
void unmakeMove(unsigned int col);
uint8_t getMoves();
bool isWin(uint64_t player_bitboard);
bool isDraw();
void printBoard();

#endif //CONNECT4_MONTE_CARLO_GAME_H
