#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
#pragma clang diagnostic ignored "-Wshift-op-parentheses"

#include "game.h"
#include "players.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <pthread.h>

// -------------------------------------------------------
// Random Player
// -------------------------------------------------------
int makeRandomMove() {
    int move = rand() % NUM_COLUMNS;
    while (!GET_BIT_8(moves, move)) {
        move = (move + 1) % NUM_COLUMNS;
    }
    makeMove(move);
    return move;
}

Player getRandomPlayer() {
    return (Player) { &makeRandomMove };
}

// -------------------------------------------------------
// User Player
// -------------------------------------------------------
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

// -------------------------------------------------------
// Monte Carlo Player
// -------------------------------------------------------
typedef struct _Node {
    struct _Node* parent;
    struct _Node* children;
    unsigned int wins;
    unsigned int num_games;
    uint8_t moveTaken;
    uint8_t children_size;
} Node;


double UCB1(Node* child) {
    double N = child->parent->num_games;
    double n = child->num_games;
    double v = ((double) child->wins) / n;
    return (n == 0) ? INT_MAX : v + sqrt(2 * log(N)/n);
}

// TODO: maybe turn into Macro
bool isLeaf(Node* node) {
    return node->children_size == 0;
}

// TODO: maybe turn into Macro
bool isRoot(Node* node) {
    return node->parent == NULL;
}

void freeNode(Node* node) {
    for (int i = 0; i < node->children_size; i++) {
        Node* child = &node->children[i];
        if (child->children_size > 0) {
            freeNode(child);
        }
    }
    free(node->children);
}

void freeTree(Node* root) {
    freeNode(root);
    free(root);
}

Node* selectNode(Node* root) {
    if (isLeaf(root)) return root;

    Node* maximizingChild = &root->children[0];
    double maxUCB1 = UCB1(maximizingChild);
    for (int i = 1; i < root->children_size; i++) {
        Node* contender = &root->children[i];
        double contenderUCB1 = UCB1(contender);
        if (contenderUCB1 > maxUCB1) {
            maxUCB1 = contenderUCB1;
            maximizingChild = contender;
        }
    }

    makeMove(maximizingChild->moveTaken);

    return selectNode(maximizingChild); // TODO: convert to iteration?
}

void expandNode(Node* node) {
    // Don't expand terminal node!
    if (GAME_OVER) return;

    // find length of children nodes
    unsigned int numChildren = 0;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        if (GET_BIT_8(moves, i)) {
            numChildren++;
        }
    }
    // calloc children nodes and update tree
    node->children = calloc(numChildren, sizeof(Node));
    node->children_size = numChildren;
    for (int i = 0; i < numChildren; i++) {
        node->children[i].parent = node;
    }
    // assign moveTaken to get to child state ; TODO: CLEAN UP THIS IMPLEMENTATION?
    int j = 0;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        if (GET_BIT_8(moves, i)) {
            node->children[j].moveTaken = i;
            j++;
        }
    }
}

int simulateRandomPlayerMove() {
    // todo: improve random playout
    int move = rand() % NUM_COLUMNS;
    while (!GET_BIT_8(moves, move)) {
        move = (move + 1) % NUM_COLUMNS;
    }
    makeMove(move);
    return move;
}

int simulatePlayout() {
    TAKE_SNAPSHOT();
    while (!GAME_OVER) {
        simulateRandomPlayerMove();
    }

    int result;
    if (isWin(bitboard[0])) {
        result = RED_WIN;
    } else if(isWin(bitboard[1])) {
        result = BLUE_WIN;
    } else {
        result = DRAW;
    }

    REVERT_TO_SNAPSHOT();
    return result;
}

void backpropogate(Node* node, int result) {
//    DUMP_SNAPSHOT();
    while (node != NULL) {
        node->num_games++;
        if ((result == RED_WIN && IS_BLUE_TO_PLAY) || (result == BLUE_WIN && IS_RED_TO_PLAY)) {
            node->wins++;
        }
        if (!isRoot(node)) {
            unmakeMove(node->moveTaken);
        }
        node = node->parent;
    }
}

int getMonteCarloMove(Node* root) {
    Node* maximizingChild = &root->children[0];
    double maxScore = ((double) maximizingChild->wins) / maximizingChild->num_games;
    for (int i = 1; i < root->children_size; i++) {
        Node* contender = &root->children[i];
        double contenderScore = ((double) contender->wins) / contender->num_games;
        if (contenderScore > maxScore) {
            maxScore = contenderScore;
            maximizingChild = contender;
        }
    }
    return (int) maximizingChild->moveTaken;
}

int makeMonteCarloMove() {
    Node* root = calloc(1, sizeof(Node));
    expandNode(root);

    for (int i = 0; i < 1000000; i++) {
        Node* node = selectNode(root);
        expandNode(node);
        int result = simulatePlayout();
        backpropogate(node, result);
    }

    int move = getMonteCarloMove(root);
    makeMove(move);
    freeTree(root);
    return move;
}

int makeMonteCarloMultiThreadedMove() {
    int NUM_THREADS = 8;

    pthread_t thr[NUM_THREADS];
    int i, rc;
    /* create a thread_data_t argument array */

    /* create threads */
    for (i = 0; i < NUM_THREADS; ++i) {
        if ((rc = pthread_create(&thr[i], NULL, thr_func, NULL))) {
            fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
            return EXIT_FAILURE;
        }
    }
    /* block until all threads complete */
    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(thr[i], NULL);
    }
}

int timedMonteCarlo() {
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    makeMonteCarloMove();

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Monte Carlo Move: took %f seconds to execute \n", cpu_time_used);
    return 0;
}

Player getMonteCarloPlayer() {
    return (Player) { &timedMonteCarlo };
//    return (Player) { &makeMonteCarloMove };
}
