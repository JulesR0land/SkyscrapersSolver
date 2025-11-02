#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "skyscrapers_lines.h"
#include "skyscrapers_cells.h"
#include "skyscrapers_utils.h"


void row_col_initializer(skyscrapers_data_t *data, int *clues) {
    data->rows = malloc(sizeof(line_info_t) * data->size);
    data->cols = malloc(sizeof(line_info_t) * data->size);

    for (int row = 0; row < data->size; row++) {
        data->rows[row].clue1 = clues[data->size * 4 - 1 - row];
        data->rows[row].clue2 = clues[data->size + row];

        data->rows[row].cells = malloc(sizeof(cell_t *) * data->size);
        for (int col = 0; col < data->size; col++) {
            data->rows[row].cells[col] = malloc(sizeof(cell_t));
            cell_initializer(data->rows[row].cells[col], data->size);
        }
    }

    for (int col = 0; col < data->size; col++) {
        data->cols[col].cells = malloc(sizeof(cell_t *) * data->size);

        data->cols[col].clue1 = clues[col];
        data->cols[col].clue2 = clues[data->size * 3 - 1 - col];

        for (int row = 0; row < data->size; row++) {
            data->cols[col].cells[row] = data->rows[row].cells[col];
            set_parent_lines(data->cols[col].cells[row], &data->rows[row], &data->cols[col]);
        }
    }
}

void row_col_copy_initializer(skyscrapers_data_t *dest, skyscrapers_data_t *data) {
    dest->rows = malloc(sizeof(line_info_t) * dest->size);
    dest->cols = malloc(sizeof(line_info_t) * dest->size);

    for (int row = 0; row < dest->size; row++) {
        dest->rows[row].clue1 = data->rows[row].clue1;
        dest->rows[row].clue2 = data->rows[row].clue2;

        dest->rows[row].cells = malloc(sizeof(cell_t *) * dest->size);
        for (int col = 0; col < dest->size; col++) {
            dest->rows[row].cells[col] = malloc(sizeof(cell_t));
            cell_copy_initializer(dest->rows[row].cells[col], data->rows[row].cells[col], dest->size);
        }
    }

    for (int col = 0; col < dest->size; col++) {
        dest->cols[col].cells = malloc(sizeof(cell_t *) * dest->size);

        dest->cols[col].clue1 = data->cols[col].clue1;
        dest->cols[col].clue2 = data->cols[col].clue2;

        for (int row = 0; row < dest->size; row++) {
            dest->cols[col].cells[row] = dest->rows[row].cells[col];
            set_parent_lines(dest->cols[col].cells[row], &dest->rows[row], &dest->cols[col]);
        }
    }
}


void row_col_copy_data(skyscrapers_data_t *dest, skyscrapers_data_t *data) {
    for (int row = 0; row < dest->size; row++) {
        for (int col = 0; col < dest->size; col++) {
            cell_copy_data(dest->rows[row].cells[col], data->rows[row].cells[col], dest->size);
        }
    }
}


void free_lines_cells(line_info_t *lines, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            free_cell(lines[i].cells[j]);
}


void free_lines(line_info_t *lines, int size) {
    for (int i = 0; i < size; i++)
        free(lines[i].cells);
    free(lines);
}


int answer_alone_poss_in_line(line_info_t *line, int size) {
    int poss_eliminated = 0;
    int *counter = calloc(size, sizeof(int));
    int *last_counted = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        for (int poss = 0; poss < size; poss++) {
            if (line->cells[i]->poss[poss] > 0) {
                counter[line->cells[i]->poss[poss] - 1]++;
                last_counted[line->cells[i]->poss[poss] - 1] = i;
            }
        }
    }

    for (int i = 0; i < size; i++)
        if (counter[i] == 1)
            poss_eliminated += set_cell_answer(line->cells[last_counted[i]], size, i + 1);

    free(counter);
    free(last_counted);

    return poss_eliminated;
}

void generate_all_comb(const line_info_t *line, const int size,
                              int *current, int **all_comb,
                              const int depth, int *index) {
    if (depth == size) {
        if (!line_legality_checker(current, size, line->clue1, line->clue2))
            return;

        for (int i = 0; i < size; i++) {
            all_comb[*index][i] = current[i];
        }
        (*index)++;
        return;
    }

    for (int i = 0; i < size; i++) {
        if (line->cells[depth]->poss[i] > 0 &&
            is_value_in_array(line->cells[depth]->poss[i], current, depth) < 0) {
            current[depth] = line->cells[depth]->poss[i];
            generate_all_comb(line, size, current, all_comb, depth + 1, index);
        }
    }
}

int eliminate_impossible_lines(line_info_t *line, int size) {
    int poss_eliminated = 0;
    int all_comb_nb = 1;
    int max_comb_nb = factorial(size);
    int **all_comb = NULL;
    int *current = malloc(sizeof(int) * size);
    int index = 0;

    for (int i = 0; i < size; i++)
        all_comb_nb *= line->cells[i]->poss_nb;

    if (all_comb_nb <= 1) {
        free(current);
        return 0;
    }

    if (all_comb_nb > max_comb_nb)
        all_comb_nb = max_comb_nb;

    all_comb = malloc(sizeof(int *) * all_comb_nb);
    for (int i = 0; i < all_comb_nb; i++)
        all_comb[i] = calloc(size, sizeof(int));

    generate_all_comb(line, size, current, all_comb, 0, &index);

    if (index == 0) {
        free(current);
        for (int i = 0; i < all_comb_nb; i++)
            free(all_comb[i]);
        free(all_comb);
        return 0;
    }

    for (int cell = 0; cell < size; cell++) {
        for (int i = 0; i < size; i++)
            current[i] = i + 1;

        for (int i = 0; i < index; i++)
            current[all_comb[i][cell] - 1] = 0;

        poss_eliminated += remove_cell_n_poss(line->cells[cell], current, size, size);
    }

    free(current);
    for (int i = 0; i < all_comb_nb; i++)
        free(all_comb[i]);
    free(all_comb);

    return poss_eliminated;
}


bool line_legality_checker(int *line, int size, int clue1, int clue2) {
    int counter = 0;
    int lowest = 0;

    if (clue1 > 0) {
        for (int i = 0; i < size; i++) {
            if (line[i] > lowest) {
                lowest = line[i];
                counter++;
                if (counter > clue1) {
                    return false;
                }
            }
        }
        if (counter != clue1)
            return false;
    }

    if (clue1 == 5) {
        for (int i = 0; i < size; i++)
            printf("%d ", line[i]);
        printf("\n");
    }

    if (clue2 == 0)
        return true;

    counter = 0;
    lowest = 0;

    for (int i = 0; i < size; i++) {
        if (line[size - 1 - i] > lowest) {
            lowest = line[size - 1 - i];
            counter++;
            if (counter > clue2)
                return false;
        }
    }

    if (counter != clue2)
        return false;

    return true;
}