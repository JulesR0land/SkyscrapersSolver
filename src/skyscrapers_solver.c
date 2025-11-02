#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "skyscrapers_solver.h"
#include "skyscrapers_lines.h"
#include "skyscrapers_cells.h"
#include "skyscrapers_data.h"
#include "skyscrapers_eliminations.h"
#include "skyscrapers_utils.h"

bool next_guess(skyscrapers_data_t *data, int try_counter) {
    for (int row = 0; row < data->size; row++) {
        for (int col = 0; col < data->size; col++) {
            if (data->rows[row].cells[col]->poss_nb > 1) {
                if (try_counter >= data->rows[row].cells[col]->poss_nb)
                    return false;
                for (int i = 0; i < data->size; i++) {
                    if (data->rows[row].cells[col]->poss[i] > 0) {
                        if (try_counter == 0) {
                            data->poss_remaining -= set_cell_answer(data->rows[row].cells[col], data->size, data->rows[row].cells[col]->poss[i]);
                            return true;
                        }
                        try_counter--;
                    }
                }
            }
        }
    }

    return false;
}

bool is_win(skyscrapers_data_t *data) {
    int *int_line = malloc(sizeof(int) * data->size);

    for (int i = 0; i < data->size; i++) {
        for (int j = 0; j < data->size; j++)
            int_line[j] = data->rows[i].cells[j]->answer;

        if (!line_legality_checker(int_line, data->size, data->rows[i].clue1, data->rows[i].clue2)) {
            free(int_line);
            return false;
        }

        for (int j = 0; j < data->size; j++)
            int_line[j] = data->cols[i].cells[j]->answer;

        if (!line_legality_checker(int_line, data->size, data->cols[i].clue1, data->cols[i].clue2)) {
            free(int_line);
            return false;
        }
    }

    free(int_line);
    return true;
}


bool recursive_solving(skyscrapers_data_t *data) {
    skyscrapers_data_t data_save;
    int poss_eliminated = 0;

    int current_poss_nb = data->poss_remaining;

    do {
        current_poss_nb = data->poss_remaining;
        for (int i = 0; i < data->size; i++) {
            poss_eliminated += eliminate_impossible_lines(&data->rows[i], data->size);
            poss_eliminated += eliminate_impossible_lines(&data->cols[i], data->size);
        }
    } while (current_poss_nb != data->poss_remaining);

    data->poss_remaining -= poss_eliminated;
    if (data->poss_remaining == data->cell_nb)
        return is_win(data);

    copy_skyscrapers_data(&data_save, data, false);
    for (int i = 0; next_guess(data, i); i++) {
        if (recursive_solving(data)) {
            free_skyscrapers_data(&data_save);
            return true;
        }
        copy_skyscrapers_data(data, &data_save, true);
    }

    free_skyscrapers_data(&data_save);

    return false;
}

void skyscrapers_solver(int square_side_size, char *str_clues) {
    int *clues = clue_scraper(str_clues, square_side_size);
    skyscrapers_data_t data;

    data.size = square_side_size;
    data.cell_nb = data.size * data.size;
    data.poss_remaining = data.cell_nb * data.size;

    row_col_initializer(&data, clues);
    free(clues);

    border_eliminations(&data);

    for (int i = 0; i < data.size; i++) {
        data.poss_remaining -= answer_alone_poss_in_line(&data.rows[i], data.size);
        data.poss_remaining -= answer_alone_poss_in_line(&data.cols[i], data.size);
    }

    if (recursive_solving(&data))
        printf("SOLVED\n");
    else
        printf("COULDN'T BE SOLVED\n");

    print_board(&data);

    free_skyscrapers_data(&data);
}