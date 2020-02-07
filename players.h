#ifndef CONNECT4_MONTE_CARLO_PLAYERS_H
#define CONNECT4_MONTE_CARLO_PLAYERS_H

typedef struct _Player {
    int (*makeMove)();
} Player;

Player getRandomPlayer();
Player getUserPlayer();
Player getMonteCarloPlayer();

#endif //CONNECT4_MONTE_CARLO_PLAYERS_H
