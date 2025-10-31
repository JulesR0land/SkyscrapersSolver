#ifndef SKYSCRAPERS_CELLS_H
#define SKYSCRAPERS_CELLS_H

#include <stdint.h>

#include "skyscrapers_structs.h"

void cell_initializer(cell_t *cell, uint32_t size);

void cell_copy_initializer(cell_t *dest, cell_t *cell, uint32_t size);
void cell_copy_data(cell_t *dest, cell_t *cell, uint32_t size);

void set_parent_lines(cell_t *cell, line_info_t *row, line_info_t *col);

void free_cell(cell_t *cell);

uint32_t set_cell_answer(cell_t *cell, uint32_t size, uint32_t answer);

uint32_t remove_cell_poss(cell_t *cell, uint32_t poss, uint32_t size);
uint32_t remove_cell_n_poss(cell_t *cell, uint32_t *poss_array, uint32_t n, uint32_t size);

#endif /* SKYSCRAPERS_CELLS_H */