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
    Player blue = getRandomPlayer();
    playMatch(red, blue);
}