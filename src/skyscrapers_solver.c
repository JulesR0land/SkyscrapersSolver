#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "skyscrapers_solver.h"
#include "skyscrapers_lines.h"
#include "skyscrapers_cells.h"

static bool next_guess(skyscrapers_data_t *data, int try_counter) {
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

static int factorial(int nb) {
    int result = 1;
    for (; nb > 1; nb--)
        result *= nb;

    return result;
}


static int is_value_in_array(int val, int *array, int size) {
    for (int i = 0; i < size; i++)
        if (array[i] == val)
            return i;
    return -1;
}


/*static bool check_unique_numbers(int *line, int size) {
    int *unique_checker = calloc(size, sizeof(int));

    for (int i = 0; i < size; i++) {
        if (unique_checker[line[i] - 1] > 0) {
            free(unique_checker);
            return false;
        }
        unique_checker[line[i] - 1]++;
    }

    free(unique_checker);
    return true;
}*/

static bool line_legality_checker(int *line, int size, int clue1, int clue2) {
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

static void generate_all_comb(const line_info_t *line, const int size,
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

static int eliminate_impossible_lines(line_info_t *line, int size) {
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

static int answer_alone_poss_in_line(line_info_t *line, int size) {
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


static int clue_poss_elimination(line_info_t *line, int size) {
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


static void border_eliminations(skyscrapers_data_t *data) {
    for (int i = 0; i < data->size; i++) {
        data->poss_remaining -= clue_poss_elimination(&data->rows[i], data->size);
        data->poss_remaining -= clue_poss_elimination(&data->cols[i], data->size);
    }
}

static bool is_win(skyscrapers_data_t *data) {
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


static void free_skyscrapers_data(skyscrapers_data_t *data) {
    free_lines_cells(data->rows, data->size);
    free_lines(data->rows, data->size);
    free_lines(data->cols, data->size);
}


static void copy_skyscrapers_data(skyscrapers_data_t *dest, skyscrapers_data_t *data, bool is_dest_initialized) {
    dest->size = data->size;
    dest->cell_nb = data->cell_nb;
    dest->poss_remaining = data->poss_remaining;

    if (is_dest_initialized)
        row_col_copy_data(dest, data);
    else
        row_col_copy_initializer(dest, data);
}


static bool recursive_solving(skyscrapers_data_t *data) {
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


void print_board(skyscrapers_data_t *data) {
    printf("\n");
    for (int col = 0; col < data->size; col++)
        printf("\t   %d", data->cols[col].clue1);

    printf("\n\t");
    for (int i = 0; i < data->size; i++)
        printf("________");
    printf("_\n\n");

    for (int row = 0; row < data->size; row++) {
        printf("%d\t|", data->rows[row].clue1);
        for (int col = 0; col < data->size; col++) {
            for (int poss = 0; poss < data->size; poss++)
                if (data->rows[row].cells[col]->poss[poss] > 0)
                    printf("%d", data->rows[row].cells[col]->poss[poss]);
            printf("\t|");
        }
        printf("\t%d", data->rows[row].clue2);
        printf("\n\t|_______");
        for (int i = 1; i < data->size; i++)
            printf("________");
        printf("|\n\n");
    }

    for (int col = 0; col < data->size; col++)
        printf("\t   %d", data->cols[col].clue2);
    printf("\n");
}

int *clue_scraper(char *str_clues, int size) {
    int clue_nb = 4 * size;
    int *clues = malloc(sizeof(int) * clue_nb);

    // clue_nb - 1 because there are no ',' for the first clue
    for (int i = 0; i < clue_nb - 1; i++) {
        clues[i] = atoi(str_clues);

        while (*str_clues != ',')
            str_clues++;
        str_clues++;
    }

    clues[clue_nb - 1] = atoi(str_clues);

    return clues;
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