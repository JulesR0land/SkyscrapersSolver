#ifndef SKYSCRAPERS_CELLS_H
#define SKYSCRAPERS_CELLS_H

#include "skyscrapers_structs.h"

void cell_initializer(cell_t *cell, int size);

void set_parent_lines(cell_t *cell, line_info_t *row, line_info_t *col);

void cell_copy_initializer(cell_t *dest, cell_t *cell, int size);
void cell_copy_data(cell_t *dest, cell_t *cell, int size);

void free_cell(cell_t *cell);

int propagate_answer(cell_t *cell, int size);
int set_cell_answer(cell_t *cell, int size, int answer);

int remove_cell_poss(cell_t *cell, int poss, int size);
int remove_cell_n_poss(cell_t *cell, int *poss_array, int n, int size);

#endif /* SKYSCRAPERS_CELLS_H */