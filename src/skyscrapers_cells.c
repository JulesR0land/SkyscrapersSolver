#include <stdlib.h>

#include "skyscrapers_cells.h"

void cell_initializer(cell_t *cell, int size) {
    cell->poss = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
        cell->poss[i] = i + 1;

    cell->poss_nb = size;
    cell->answer = 0;
}


void set_parent_lines(cell_t *cell, line_info_t *row, line_info_t *col) {
    cell->parent_row = row;
    cell->parent_col = col;
}

void cell_copy_initializer(cell_t *dest, cell_t *cell, int size) {
    dest->poss = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
        dest->poss[i] = cell->poss[i];

    dest->poss_nb = cell->poss_nb;
    dest->answer = cell->answer;
}

void cell_copy_data(cell_t *dest, cell_t *cell, int size) {
    for (int i = 0; i < size; i++)
        dest->poss[i] = cell->poss[i];

    dest->poss_nb = cell->poss_nb;
    dest->answer = cell->answer;
}


void free_cell(cell_t *cell) {
    free(cell->poss);
    free(cell);
}


int propagate_answer(cell_t *cell, int size) {
    int removed_poss = 0;

    for (int i = 0; i < size; i++) {
        removed_poss += remove_cell_poss(cell->parent_row->cells[i], cell->answer, size);
        removed_poss += remove_cell_poss(cell->parent_col->cells[i], cell->answer, size);
    }

    return removed_poss;
}


int set_cell_answer(cell_t *cell, int size, int answer) {
    int removed_poss = 0;

    for (int i = 0; i < size; i++)
        cell->poss[i] = 0;

    cell->poss[answer - 1] = answer;
    removed_poss = cell->poss_nb - 1;
    cell->poss_nb = 1;
    cell->answer = answer;

    removed_poss += propagate_answer(cell, size);

    return removed_poss;
}


int remove_cell_poss(cell_t *cell, int poss, int size) {
    if (cell->poss_nb <= 1 || cell->poss[poss - 1] == 0)
        return 0;

    int removed_poss = 1;
    int i = 0;

    cell->poss[poss - 1] = 0;
    cell->poss_nb -= 1;

    if (cell->poss_nb == 1) {
        for (; cell->poss[i] == 0; i++);

        cell->answer = i + 1;
        removed_poss += propagate_answer(cell, size);
    }

    return removed_poss;
}

int remove_cell_n_poss(cell_t *cell, int *poss_array, int n, int size) {
    int removed_poss = 0;
    int i = 0;

    for (int i = 0; i < n; i++) {
        if (poss_array[i] > 0 && cell->poss[poss_array[i] - 1] > 0) {
            cell->poss[poss_array[i] - 1] = 0;
            removed_poss++;
        }
    }

    if (removed_poss > 0) {
        cell->poss_nb -= removed_poss;
        if (cell->poss_nb == 1) {
            for (; cell->poss[i] == 0; i++);

            cell->answer = i + 1;
            removed_poss += propagate_answer(cell, size);
        }
    }

    return removed_poss;
}
