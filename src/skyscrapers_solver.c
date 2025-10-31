#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


#include "skyscrapers_solver.h"
#include "skyscrapers_lines.h"
#include "skyscrapers_cells.h"

/*
#include "skyscrapers_board.h"
#include "skyscrapers_clues.h"
#include "skyscrapers_checkers.h"
#include "skyscrapers_tools.h"

bool line_clue_checker(uint32_t *line, uint32_t ss, uint32_t clue1, uint32_t clue2) {
    uint32_t counter = 0;
    uint32_t lowest = 0;

    for (uint32_t i = 0; i < ss; i++) {
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

    counter = 0;
    lowest = 0;

    for (uint32_t i = 0; i < ss; i++) {
        if (line[ss - 1 - i] > lowest) {
            lowest = line[ss - 1 - i];
            counter++;
            if (counter > clue2) {
                return false;
            }
        }
    }

    if (counter != clue2)
        return false;

    return true;
}


void generate_all_comb(const board_cell_t *line, const uint32_t line_len,
                              uint32_t *current, uint32_t **all_comb,
                              const uint32_t depth, uint32_t *index,
                              uint32_t clue1, uint32_t clue2) {
    if (depth == line_len) {
        if (!line_clue_checker(current, line_len, clue1, clue2))
            return;

        for (uint32_t i = 0; i < line_len; i++) {
            all_comb[*index][i] = current[i];
        }
        (*index)++;
        return;
    }

    for (uint32_t i = 0; i < line_len; i++) {
        if (line[depth].poss[i] > 0 &&
            is_value_in_array(line[depth].poss[i], current, depth) == UINT32_MAX) {
            current[depth] = line[depth].poss[i];
            generate_all_comb(line, line_len, current, all_comb, depth + 1, index, clue1, clue2);
        }
    }
}


void try_possible_row(board_info_t *b, uint32_t row) {
    uint32_t all_comb_nb = 1;
    uint32_t facto = factorial(b->ss);
    uint32_t **all_comb = NULL;
    uint32_t *current = malloc(sizeof(int) * b->ss);
    board_cell_t *line = malloc(sizeof(board_cell_t) * b->ss);
    uint32_t clue1 = b->clues[b->ss * 4 - 1 - row].clue;
    uint32_t clue2 = b->clues[b->ss + row].clue;
    uint32_t index = 0;

    for (uint32_t col = 0; col < b->ss; col++) {
        line[col] = b->board[row][col];
        all_comb_nb *= b->board[row][col].poss_nb;
    }

    if (all_comb_nb <= 1) {
        free(current);
        free(line);
        return;
    }

    if (all_comb_nb > facto)
        all_comb_nb = facto;

    all_comb = malloc(sizeof(uint32_t *) * all_comb_nb);
    for (uint32_t i = 0; i < all_comb_nb; i++)
        all_comb[i] = calloc(b->ss, sizeof(uint32_t));

    generate_all_comb(line, b->ss, current, all_comb, 0, &index, clue1, clue2);

    if (all_comb[0][0] != 0) {
        for (uint32_t col = 0; col < b->ss; col++) {
            for (uint32_t i = 0; i < b->ss; i++)
                current[i] = i + 1;
            for (uint32_t i = 0; all_comb[i][0] != 0 && i < all_comb_nb; i++) {
                current[all_comb[i][col] - 1] = 0;
            }

            remove_cell_n_poss(b, row, col, current, b->ss);
        }
    }


    for (uint32_t i = 0; i < all_comb_nb; i++)
        free(all_comb[i]);
    free(all_comb);
    free(line);
}

void try_possible_col(board_info_t *b, uint32_t col) {
    uint32_t all_comb_nb = 1;
    uint32_t **all_comb = NULL;
    uint32_t *current = malloc(sizeof(int) * b->ss);
    board_cell_t *line = malloc(sizeof(board_cell_t) * b->ss);
    uint32_t clue1 = b->clues[col].clue;
    uint32_t clue2 = b->clues[b->ss * 3 - 1 - col].clue;
    uint32_t index = 0;

    for (uint32_t row = 0; row < b->ss; row++) {
        line[row] = b->board[row][col];
        all_comb_nb *= b->board[row][col].poss_nb;
    }

    if (all_comb_nb <= 1) {
        free(current);
        free(line);
        return;
    }

    all_comb = malloc(sizeof(uint32_t *) * all_comb_nb);
    for (uint32_t i = 0; i < all_comb_nb; i++)
        all_comb[i] = calloc(b->ss, sizeof(uint32_t));

    generate_all_comb(line, b->ss, current, all_comb, 0, &index, clue1, clue2);

    for (uint32_t i = 0; i < b->ss; i++)
        current[i] = 0;

    if (all_comb[0][0] != 0) {
        for (uint32_t row = 0; row < b->ss; row++) {
            for (uint32_t i = 0; i < b->ss; i++)
                current[i] = i + 1;
            for (uint32_t i = 0; all_comb[i][0] != 0 && i < all_comb_nb; i++) {
                current[all_comb[i][row] - 1] = 0;
            }

            remove_cell_n_poss(b, row, col, current, b->ss);
        }
    }

    for (uint32_t i = 0; i < all_comb_nb; i++)
        free(all_comb[i]);
    free(all_comb);
    free(line);
}

void try_possible_lines(board_info_t *b) {
    for (uint32_t i = 0; i < b->ss; i++) {
        try_possible_row(b, i);
        try_possible_col(b, i);
    }
}


void check_alone_poss_row(board_info_t *b, uint32_t row) {
    uint32_t *counter = calloc(b->ss, sizeof(uint32_t));
    uint32_t *last_counted = calloc(b->ss, sizeof(uint32_t));

    for (uint32_t col = 0; col < b->ss; col++)
        if (b->board[row][col].poss_nb > 1)
            for (uint32_t i = 0; i < b->ss; i++)
                if (b->board[row][col].poss[i] > 0) {
                    counter[b->board[row][col].poss[i] - 1]++;
                    last_counted[b->board[row][col].poss[i] - 1] = col;
                }

    for (uint32_t i = 0; i < b->ss; i++)
        if (counter[i] == 1)
            set_answer(b, row, last_counted[i], i + 1);

    free(counter);
    free(last_counted);
}

void check_alone_poss_col(board_info_t *b, uint32_t col) {
    uint32_t *counter = calloc(b->ss, sizeof(uint32_t));
    uint32_t *last_counted = calloc(b->ss, sizeof(uint32_t));

    for (uint32_t row = 0; row < b->ss; row++)
        if (b->board[row][col].poss_nb > 1)
            for (uint32_t i = 0; i < b->ss; i++)
                if (b->board[row][col].poss[i] > 0) {
                    counter[b->board[row][col].poss[i] - 1]++;
                    last_counted[b->board[row][col].poss[i] - 1] = row;
                }

    for (uint32_t i = 0; i < b->ss; i++)
        if (counter[i] == 1)
            set_answer(b, last_counted[i], col, i + 1);

    free(counter);
    free(last_counted);
}

void check_alone_poss(board_info_t *b) {
    for (uint32_t row = 0; row < b->ss; row++) {
        for (uint32_t col = 0; col < b->ss; col++) {
            check_alone_poss_row(b, row);
            check_alone_poss_col(b, col);
        }
    }
}


void line_answers_initializer(board_info_t *b) {
    b->row_answers = malloc(sizeof(line_answers_t) * b->ss);
    b->col_answers = malloc(sizeof(line_answers_t) * b->ss);

    for (uint32_t i = 0; i < b->ss; i++) {
        b->row_answers[i].answers = calloc(b->ss, sizeof(uint32_t));
        b->row_answers[i].answer_nb = 0;

        b->col_answers[i].answers = calloc(b->ss, sizeof(uint32_t));
        b->col_answers[i].answer_nb = 0;
    }
}


bool guess_next_answer(board_info_t *b, uint32_t poss_guess) {
    uint32_t row = 0;
    uint32_t col = 0;

    for (row = 0; row < b->ss; row++) {
        for (col = 0; col < b->ss; col++) {
            if (b->board[row][col].poss_nb > 1)
                break;
        }
        if (col < b->ss)
            break;
    }

    if (row == b->ss || poss_guess >= b->board[row][col].poss_nb)
        return false;

    for (uint32_t i = 0; i < b->ss; i++) {
        if (b->board[row][col].poss[i] > 0) {
            if (poss_guess == 0) {
                set_answer(b, row, col, b->board[row][col].poss[i]);
                return true;
            }
            poss_guess--;
        }
    }

    return false;
}

void copy_board_info(board_info_t *orig, board_info_t *copy) {
    copy->ss = orig->ss;
    copy->clue_nb = orig->clue_nb;
    copy->remaining_poss_nb = orig->remaining_poss_nb;

    copy->answer_nb = orig->answer_nb;
    copy->win = orig->win;

    copy->board = board_initializer(orig->ss);
    copy_board_cells(orig->board, copy->board, orig->ss);

    copy->clues = orig->clues;

    line_answers_initializer(copy);

    for (uint32_t i = 0; i < orig->ss; i++) {
        copy->row_answers[i].answer_nb = orig->row_answers[i].answer_nb;
        for (uint32_t j = 0; j < orig->ss; j++) {
            copy->row_answers[i].answers[j] = orig->row_answers[i].answers[j];
        }
    }

    for (uint32_t i = 0; i < orig->ss; i++) {
        copy->col_answers[i].answer_nb = orig->col_answers[i].answer_nb;
        for (uint32_t j = 0; j < orig->ss; j++) {
            copy->col_answers[i].answers[j] = orig->col_answers[i].answers[j];
        }
    }
}


bool recursive_it(board_info_t *b) {
    board_info_t copy;
    uint32_t current_poss_nb = b->remaining_poss_nb;

    do {
        current_poss_nb = b->remaining_poss_nb;
        try_possible_lines(b);
        //printf("4");
        //print_board(b);
    } while (current_poss_nb != b->remaining_poss_nb);

    if (b->answer_nb == b->win)
        return check_win(b);

    copy_board_info(b, &copy);
    for (uint32_t i = 0; guess_next_answer(b, i); i++) {
        if (recursive_it(b))
            return true;
        copy_board_info(&copy, b);
    }

    return false;
}

*/

static uint32_t factorial(uint32_t nb) {
    uint32_t result = 1;
    for (; nb > 1; nb--)
        result *= nb;

    return result;
}


static uint32_t is_value_in_array(uint32_t val, uint32_t *array, uint32_t size) {
    for (uint32_t i = 0; i < size; i++)
        if (array[i] == val)
            return i;
    return UINT32_MAX;
}

static uint32_t eliminate_impossible_lines(line_info_t *line, uint32_t size) {
    uint32_t all_comb_nb = 1;
    uint32_t max_comb_nb = factorial(size);
    uint32_t **all_comb = NULL;
    uint32_t *current = malloc(sizeof(int) * size);
    board_cell_t *line = malloc(sizeof(board_cell_t) * size);
    uint32_t index = 0;

    for (uint32_t col = 0; col < b->ss; col++) {
        line[col] = b->board[row][col];
        all_comb_nb *= b->board[row][col].poss_nb;
    }

    if (all_comb_nb <= 1) {
        free(current);
        free(line);
        return;
    }

    if (all_comb_nb > max_comb_nb)
        all_comb_nb = max_comb_nb;

    all_comb = malloc(sizeof(uint32_t *) * all_comb_nb);
    for (uint32_t i = 0; i < all_comb_nb; i++)
        all_comb[i] = calloc(size, sizeof(uint32_t));

    generate_all_comb(line, size, current, all_comb, 0, &index, line->clue1, line->clue2);

    if (all_comb[0][0] != 0) {
        for (uint32_t col = 0; col < size; col++) {
            for (uint32_t i = 0; i < size; i++)
                current[i] = i + 1;
            for (uint32_t i = 0; all_comb[i][0] != 0 && i < all_comb_nb; i++) {
                current[all_comb[i][col] - 1] = 0;
            }

            remove_cell_n_poss(b, row, col, current, size);
        }
    }


    for (uint32_t i = 0; i < all_comb_nb; i++)
        free(all_comb[i]);
    free(all_comb);
    free(line);
}

static uint32_t answer_alone_poss_in_line(line_info_t *line, uint32_t size) {
    uint32_t poss_eliminated = 0;
    uint32_t *counter = calloc(size, sizeof(uint32_t));
    uint32_t *last_counted = calloc(size, sizeof(uint32_t));

    for (uint32_t i = 0; i < size; i++) {
        for (uint32_t poss = 0; poss < size; poss++) {
            if (line->cells[i]->poss[poss] > 0) {
                counter[line->cells[i]->poss[poss] - 1]++;
                last_counted[line->cells[i]->poss[poss] - 1] = i;
            }
        }
    }

    for (uint32_t i = 0; i < size; i++)
        if (counter[i] == 1)
            poss_eliminated += set_cell_answer(line->cells[last_counted[i]], size, i + 1);

    free(counter);
    free(last_counted);

    return poss_eliminated;
}


static uint32_t clue_poss_elimination(line_info_t *line, uint32_t size) {
    const uint32_t last_elem = size - 1;
    uint32_t poss_eliminated = 0;
    // eliminate poss > ss - clue + 1 + border distance (0 based)
    uint32_t to_elim = size - line->clue1 + 1;
    uint32_t *poss_array = malloc(sizeof(uint32_t) * size);

    for (uint32_t i = 0; i < size; i++)
        poss_array[i] = size - i;

    if (line->clue1 == 1)
        poss_eliminated += set_cell_answer(line->cells[0], size, size);
    else
        poss_eliminated += remove_cell_n_poss(line->cells[0], poss_array, size - to_elim, size);


    to_elim = size - line->clue2 + 1;

    if (line->clue2 == 1)
        poss_eliminated += set_cell_answer(line->cells[last_elem], size, size);
    else
        poss_eliminated += remove_cell_n_poss(line->cells[last_elem], poss_array, size - to_elim, size);

    free(poss_array);
    return poss_eliminated;
}


static void border_eliminations(skyscrapers_data_t *data) {
    for (uint32_t i = 0; i < data->size; i++) {
        data->poss_remaining -= clue_poss_elimination(&data->rows[i], data->size);
        data->poss_remaining -= clue_poss_elimination(&data->cols[i], data->size);
    }
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

    uint32_t current_poss_nb = data->poss_remaining;

    do {
        current_poss_nb = data->poss_remaining;
        for (uint32_t i = 0; i < data->size; i++) {
            eliminate_impossible_lines(&data->rows[i], data->size);
            eliminate_impossible_lines(&data->cols[i], data->size);
        }
    } while (current_poss_nb != data->poss_remaining);

    if (data->poss_remaining == data->cell_nb)
        return true;//is_win(b);

    copy_skyscrapers_data(&data_save, data, false);
    for (uint32_t i = 0; false/*guess_next_answer(data, i)*/; i++) {
        if (recursive_solving(data)) {
            free_skyscrapers_data(&data_save);
            return true;
        }
        copy_skyscrapers_data(data, &data_save, true);
    }

    free_skyscrapers_data(&data_save);
    return false;
}


static void print_board(skyscrapers_data_t *data) {
    printf("\n");
    for (uint32_t col = 0; col < data->size; col++)
        printf("\t   %d", data->cols[col].clue1);

    printf("\n\t");
    for (uint32_t i = 0; i < data->size; i++)
        printf("________");
    printf("_\n\n");

    for (uint32_t row = 0; row < data->size; row++) {
        printf("%d\t|", data->rows[row].clue1);
        for (uint32_t col = 0; col < data->size; col++) {
            for (uint32_t poss = 0; poss < data->size; poss++)
                if (data->rows[row].cells[col]->poss[poss] > 0)
                    printf("%d", data->rows[row].cells[col]->poss[poss]);
            printf("\t|");
        }
        printf("\t%d", data->rows[row].clue2);
        printf("\n\t|_______");
        for (uint32_t i = 1; i < data->size; i++)
            printf("________");
        printf("|\n\n");
    }

    for (uint32_t col = 0; col < data->size; col++)
        printf("\t   %d", data->cols[col].clue2);
    printf("\n");
}

uint32_t *clue_scraper(char *str_clues, uint32_t size) {
    uint32_t clue_nb = 4 * size;
    uint32_t *clues = malloc(sizeof(uint32_t) * clue_nb);

    // clue_nb - 1 because there are no ',' for the first clue
    for (uint32_t i = 0; i < clue_nb - 1; i++) {
        clues[i] = atoi(str_clues);

        while (*str_clues != ',')
            str_clues++;
        str_clues++;
    }

    clues[clue_nb - 1] = atoi(str_clues);

    return clues;
}

void skyscrapers_solver(uint32_t square_side_size, char *str_clues) {
    uint32_t *clues = clue_scraper(str_clues, square_side_size);
    skyscrapers_data_t data;

    data.size = square_side_size;
    data.cell_nb = data.size * data.size;
    data.poss_remaining = data.cell_nb * data.size;

    row_col_initializer(&data, clues);
    free(clues);

    border_eliminations(&data);

    for (uint32_t i = 0; i < data.size; i++) {
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

/*
void skyscrapers_solver(uint32_t square_size, char *str_clues) {
    board_info_t b;

    b.ss = square_size;
    b.answer_nb = 0;
    b.win = b.ss * b.ss;
    b.clue_nb = 4 * b.ss;
    b.remaining_poss_nb = b.ss * b.ss * b.ss;
    b.board = board_initializer(b.ss);
    b.clues = clue_parser(str_clues, b.ss);


    line_answers_initializer(&b);
    //printf("1");
    //print_board(&b);

    clue_border_elimination(&b);
    //printf("2");
    //print_board(&b);

    check_alone_poss(&b);
    //printf("3");
    //print_board(&b);

    if (recursive_it(&b))
        printf("WONDERFUL, YOU WIN !!!\n");
    else
        printf("YO IT'S IMPOSSIBLE WTF !\n");
    //printf("5");
    print_board(&b);

    free_board_info(&b);
}
*/