#include <stdlib.h>

#include "skyscrapers_board.h"


void copy_board_cells(board_cell_t **orig, board_cell_t **copy, uint32_t ss) {
    for (uint32_t row = 0; row < ss; row++) {
        for (uint32_t col = 0; col < ss; col++) {
            copy[row][col].poss_nb = orig[row][col].poss_nb;
            copy[row][col].answer = orig[row][col].answer;
            for (uint32_t i = 0; i < ss; i++) {
                copy[row][col].poss[i] = orig[row][col].poss[i];
            }
        }
    }
}


void board_cell_constructor(board_cell_t *cell, uint32_t square_size) {
    cell->poss = malloc(sizeof(int) * square_size);

    for (uint32_t poss_i = 0; poss_i < square_size; poss_i++)
        cell->poss[poss_i] = poss_i + 1;

    cell->poss_nb = square_size;
    cell->answer = 0;
}


board_cell_t **board_initializer(uint32_t square_size) {
    board_cell_t **board = malloc(sizeof(board_cell_t *) * square_size);

    for (uint32_t row = 0; row < square_size; row++) {
        board[row] = malloc(sizeof(board_cell_t) * square_size);
        for (uint32_t col = 0; col < square_size; col++) {
            board_cell_constructor(&board[row][col], square_size);
        }
    }

    return board;
}