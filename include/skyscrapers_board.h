#ifndef SKYSCRAPERS_BOARD_H
#define SKYSCRAPERS_BOARD_H

#include "skyscrapers_structs.h"


void copy_board_cells(board_cell_t **orig, board_cell_t **copy, uint32_t ss);

void board_cell_constructor(board_cell_t *cell, uint32_t square_size);

board_cell_t **board_initializer(uint32_t square_size);


#endif /* SKYSCRAPERS_BOARD_H */