#include <stdlib.h>
#include <stdbool.h>

#include "skyscrapers_clues.h"
#include "skyscrapers_tools.h"


void clue_border_elimination_algo(board_info_t *b, clues_info_t *clue) {
    // eliminate poss >= ss - clue + 2 + border distance (0 based)
    uint32_t to_elim = b->ss - (b->ss - clue->clue + 2) + 1;
    uint32_t row = clue->row;
    uint32_t col = clue->col;
    uint32_t *poss_array = malloc(sizeof(uint32_t) * b->ss);

    for (uint32_t i = 0; i < b->ss; i++)
        poss_array[i] = b->ss - i;

    for (uint32_t i = 0; to_elim > 0; i++) {
        row = clue->row + clue->row_d * i;
        col = clue->col + clue->col_d * i;
        remove_cell_n_poss(b, row, col, poss_array, to_elim);
        to_elim--;
    }

    free(poss_array);
}

void clue_border_elimination(board_info_t *b) {
    for (uint32_t i = 0; i < b->clue_nb; i++) {
        if (b->clues[i].clue > 1) {
            clue_border_elimination_algo(b, &b->clues[i]);
        } else if (b->clues[i].clue == 1) {
            set_answer(b, b->clues[i].row, b->clues[i].col, b->ss);
        }
    }
}


void clue_constructor(clues_info_t *clue_info, uint32_t clue,
                      uint32_t row, uint32_t col,
                      direction_t direction) {
    clue_info->clue = clue;

    clue_info->row = row;
    clue_info->col = col;

    switch (direction) {
        case UP:
            clue_info->row_d = -1;
            clue_info->col_d = 0;
            break;
        case RIGHT:
            clue_info->row_d = 0;
            clue_info->col_d = 1;
            break;
        case DOWN:
            clue_info->row_d = 1;
            clue_info->col_d = 0;
            break;
        case LEFT:
            clue_info->row_d = 0;
            clue_info->col_d = -1;
            break;
    }
}

clues_info_t *clue_parser(char *str_clues, uint32_t ss) {
    uint32_t clue_nb = 4 * ss;
    clues_info_t *clues = malloc(sizeof(clues_info_t) * clue_nb);

    // clue_nb - 1 because there are no ',' for the first clue
    for (uint32_t i = 0; i < clue_nb - 1; i++) {

        if (i < ss)
            clue_constructor(&clues[i], atoi(str_clues),
                             0, i, DOWN);
        else if (i < 2 * ss)
            clue_constructor(&clues[i], atoi(str_clues),
                             i - ss, ss - 1, LEFT);
        else if (i < 3 * ss)
            clue_constructor(&clues[i], atoi(str_clues),
                             ss - 1, (ss - 1) - (i - (2 * ss)), UP);
        else
            clue_constructor(&clues[i], atoi(str_clues),
                             (ss - 1) - (i - (3 * ss)), 0, RIGHT);
        while (str_clues[0] != ',')
            str_clues++;
        str_clues++;
    }

    clue_constructor(&clues[clue_nb - 1], atoi(str_clues),
                     (ss - 1) - (clue_nb - 1 - (3 * ss)), 0, RIGHT);

    return clues;
}