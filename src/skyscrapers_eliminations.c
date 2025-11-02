#include <stdlib.h>

#include "skyscrapers_eliminations.h"
#include "skyscrapers_cells.h"
#include "skyscrapers_structs.h"


int clue_poss_elimination(line_info_t *line, int size) {
    const int last_elem = size - 1;
    int poss_eliminated = 0;
    // eliminate poss > ss - clue + 1 + border distance (0 based)
    int to_elim = 0;
    int *poss_array = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++)
        poss_array[i] = size - i;

    if (line->clue1 != 0) {
        to_elim = size - line->clue1 + 1;
        if (line->clue1 == 1)
            poss_eliminated += set_cell_answer(line->cells[0], size, size);
        else
            for (int i = 0; to_elim < size; i++) {
                poss_eliminated += remove_cell_n_poss(line->cells[i], poss_array, size - to_elim, size);
                to_elim++;
            }
    }

    if (line->clue2 != 0) {
        to_elim = size - line->clue2 + 1;

        if (line->clue2 == 1)
            poss_eliminated += set_cell_answer(line->cells[last_elem], size, size);
        else
            for (int i = size - 1; to_elim < size; i--) {
                poss_eliminated += remove_cell_n_poss(line->cells[i], poss_array, size - to_elim, size);
                to_elim++;
            }
    }

    free(poss_array);
    return poss_eliminated;
}


void border_eliminations(skyscrapers_data_t *data) {
    for (int i = 0; i < data->size; i++) {
        data->poss_remaining -= clue_poss_elimination(&data->rows[i], data->size);
        data->poss_remaining -= clue_poss_elimination(&data->cols[i], data->size);
    }
}