#pragma ide diagnostic ignored "cert-msc32-c"

#include <stdlib.h>
#include <time.h>
#include "tournament.h"

void init() {
    srand(time(NULL));
}

int main() {
    init();

    Player red = getUserPlayer();
//    Player red = getMonteCarloPlayer();
    Player blue = getMonteCarloPlayer();
    playMatches(red, blue, 1);
}

// TODO: BUGFIX: You are calloc-ing a bunch of memory, this memory needs to be freed (use leaks application)

// TODO: ENHANCEMENT: Add multi-threading for Monte Carlo Player
// TODO: ENHANCEMENT: Add some sort of benchmarking to gauge optimizations you make

// TODO: FEATURE: Add an AlphaZeroPlayer
// TODO: (OPTIONAL) FEATURE: Add a StrongMinimaxPlayer