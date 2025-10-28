#include "skyscrapers_solver.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


static uint32_t factorial(uint32_t nb) {
    uint32_t result = 1;
    for (; nb > 1; nb--)
        result *= nb;

    return result;
}


static bool line_clue_checker(uint32_t *line, uint32_t ss, uint32_t clue1, uint32_t clue2) {
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


static uint32_t is_value_in_array(uint32_t val, uint32_t *array, uint32_t size) {
    for (uint32_t i = 0; i < size; i++)
        if (array[i] == val)
            return i;
    return UINT32_MAX;
}


static void generate_all_comb(const board_cell_t *line, const uint32_t line_len,
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


static void try_possible_row(board_info_t *b, uint32_t row) {
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

static void try_possible_col(board_info_t *b, uint32_t col) {
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

static void try_possible_lines(board_info_t *b) {
    for (uint32_t i = 0; i < b->ss; i++) {
        try_possible_row(b, i);
        try_possible_col(b, i);
    }
}


static void check_alone_poss_row(board_info_t *b, uint32_t row) {
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

static void check_alone_poss_col(board_info_t *b, uint32_t col) {
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

static void check_alone_poss(board_info_t *b) {
    for (uint32_t row = 0; row < b->ss; row++) {
        for (uint32_t col = 0; col < b->ss; col++) {
            check_alone_poss_row(b, row);
            check_alone_poss_col(b, col);
        }
    }
}


void elim_answered_poss_col(board_info_t *b, uint32_t col) {
    for (uint32_t row = 0; row < b->ss; row++) {
        for (uint32_t i = 0; i < b->col_answers[col].answer_nb; i++)
            remove_cell_poss(b, row, col, b->col_answers[col].answers[i]);
    }
}

void elim_answered_poss_row(board_info_t *b, uint32_t row) {
    for (uint32_t col = 0; col < b->ss; col++) {
        for (uint32_t i = 0; i < b->row_answers[row].answer_nb; i++)
            remove_cell_poss(b, row, col, b->row_answers[row].answers[i]);
    }
}

void set_answer(board_info_t *b, uint32_t row, uint32_t col, uint32_t answer) {
    if (b->board[row][col].answer == answer)
        return;

    for (uint32_t i = 0; i < b->ss; i++)
        b->board[row][col].poss[i] = 0;

    b->board[row][col].poss[answer - 1] = answer;
    b->board[row][col].poss_nb = 1;
    b->board[row][col].answer = answer;

    b->row_answers[row].answers[b->row_answers[row].answer_nb] =
        b->board[row][col].answer;
    b->row_answers[row].answer_nb++;

    b->col_answers[col].answers[b->col_answers[col].answer_nb] =
        b->board[row][col].answer;
    b->col_answers[col].answer_nb++;

    b->answer_nb++;
    if (b->answer_nb == b->win)
        return;

    elim_answered_poss_row(b, row);
    elim_answered_poss_col(b, col);
}


static bool check_poss_to_answer(board_info_t *b, uint32_t row, uint32_t col) {
    if (b->board[row][col].poss_nb != 1)
        return false;

    uint32_t i = 0;
    for (; b->board[row][col].poss[i] == 0; i++);

    b->board[row][col].answer = b->board[row][col].poss[i];

    b->row_answers[row].answers[b->row_answers[row].answer_nb] = b->board[row][col].answer;
    b->row_answers[row].answer_nb++;

    b->col_answers[col].answers[b->col_answers[col].answer_nb] =
        b->board[row][col].answer;
    b->col_answers[col].answer_nb++;

    b->answer_nb++;
    if (b->answer_nb == b->win)
        return true;

    elim_answered_poss_row(b, row);
    elim_answered_poss_col(b, col);

    return true;
}


uint32_t remove_cell_n_poss(board_info_t *b, uint32_t row, uint32_t col,
                                   uint32_t *poss_array, uint32_t n) {
    uint32_t poss_removed = 0;

    for (uint32_t i = 0; i < n; i++) {
        if (poss_array[i] > 0 && b->board[row][col].poss[poss_array[i] - 1] > 0) {
            b->board[row][col].poss[poss_array[i] - 1] = 0;
            poss_removed++;
        }
    }

    if (poss_removed > 0) {
        b->board[row][col].poss_nb -= poss_removed;
        b->remaining_poss_nb -= poss_removed;
        check_poss_to_answer(b, row, col);
    }

    return b->board[row][col].poss_nb;
}

int remove_cell_poss(board_info_t *b, uint32_t row, uint32_t col, int possibility) {
    if (b->board[row][col].poss_nb <= 1 ||
        b->board[row][col].poss[possibility - 1] == 0)
        return b->board[row][col].poss_nb;

    b->board[row][col].poss[possibility - 1] = 0;
    b->board[row][col].poss_nb -= 1;
    b->remaining_poss_nb -= 1;

    check_poss_to_answer(b, row, col);

    return b->board[row][col].poss_nb;
}


static void clue_border_elimination_algo(board_info_t *b, clues_info_t *clue) {
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

static void clue_border_elimination(board_info_t *b) {
    for (uint32_t i = 0; i < b->clue_nb; i++) {
        if (b->clues[i].clue > 1) {
            clue_border_elimination_algo(b, &b->clues[i]);
        } else if (b->clues[i].clue == 1) {
            set_answer(b, b->clues[i].row, b->clues[i].col, b->ss);
        }
    }
}


static void print_board(board_info_t *b) {
    printf("\n");
    for (uint32_t i = 0; i < b->ss; i++)
        printf("\t   %d", b->clues[i].clue);
    printf("\n\t_________________________________________________________\n\n");
    for (uint32_t row = 0; row < b->ss; row++) {
        printf("%d\t|", b->clues[b->ss * 3 + (b->ss - 1 - row)].clue);
        for (uint32_t col = 0; col < b->ss; col++) {
            for (uint32_t i = 0; i < b->ss; i++) {
                if (b->board[row][col].poss[i] > 0)
                    printf("%d", b->board[row][col].poss[i]);
            }
            printf("\t|");
        }
        printf("\t%d", b->clues[b->ss + row].clue);
        printf("\n\t_________________________________________________________\n\n");
    }
    for (uint32_t i = 0; i < b->ss; i++)
        printf("\t   %d", b->clues[b->ss * 2 + (b->ss - 1 - i)].clue);
    printf("\n--------------------------------------------\n");
}


static void line_answers_initializer(board_info_t *b) {
    b->row_answers = malloc(sizeof(line_answers_t) * b->ss);
    b->col_answers = malloc(sizeof(line_answers_t) * b->ss);

    for (uint32_t i = 0; i < b->ss; i++) {
        b->row_answers[i].answers = calloc(b->ss, sizeof(uint32_t));
        b->row_answers[i].answer_nb = 0;

        b->col_answers[i].answers = calloc(b->ss, sizeof(uint32_t));
        b->col_answers[i].answer_nb = 0;
    }
}


static void clue_constructor(clues_info_t *clue_info, uint32_t clue,
                             uint32_t row, uint32_t col,
                             direction_t direction, uint32_t ss) {
    clue_info->clue = clue;
    clue_info->answers = malloc(sizeof(int) * ss);

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

static clues_info_t *clue_parser(char *str_clues, uint32_t ss) {
    uint32_t clue_nb = 4 * ss;
    clues_info_t *clues = malloc(sizeof(clues_info_t) * clue_nb);

    // clue_nb - 1 because there are no ',' for the first clue
    for (uint32_t i = 0; i < clue_nb - 1; i++) {

        if (i < ss)
            clue_constructor(&clues[i], atoi(str_clues),
                             0, i, DOWN, ss);
        else if (i < 2 * ss)
            clue_constructor(&clues[i], atoi(str_clues),
                             i - ss, ss - 1, LEFT, ss);
        else if (i < 3 * ss)
            clue_constructor(&clues[i], atoi(str_clues),
                             ss - 1, (ss - 1) - (i - (2 * ss)), UP, ss);
        else
            clue_constructor(&clues[i], atoi(str_clues),
                             (ss - 1) - (i - (3 * ss)), 0, RIGHT, ss);
        while (str_clues[0] != ',')
            str_clues++;
        str_clues++;
    }

    clue_constructor(&clues[clue_nb - 1], atoi(str_clues),
                     (ss - 1) - (clue_nb - 1 - (3 * ss)), 0, RIGHT, ss);

    return clues;
}


static void board_cell_constructor(board_cell_t *cell, uint32_t square_size) {
    cell->poss = malloc(sizeof(int) * square_size);

    for (uint32_t poss_i = 0; poss_i < square_size; poss_i++)
        cell->poss[poss_i] = poss_i + 1;

    cell->poss_nb = square_size;
    cell->answer = 0;
}

static board_cell_t **board_initializer(uint32_t square_size) {
    board_cell_t **board = malloc(sizeof(board_cell_t *) * square_size);

    for (uint32_t row = 0; row < square_size; row++) {
        board[row] = malloc(sizeof(board_cell_t) * square_size);
        for (uint32_t col = 0; col < square_size; col++) {
            board_cell_constructor(&board[row][col], square_size);
        }
    }

    return board;
}


static bool clue_checker(board_info_t *b, uint32_t clue_index) {
    uint32_t counter = 0;
    uint32_t lowest = 0;
    uint32_t row = b->clues[clue_index].row;
    uint32_t col = b->clues[clue_index].col;

    if (b->clues[clue_index].clue == 0)
        return true;

    for (uint32_t i = 0; i < b->ss; i++) {
        if (b->board[row][col].answer > lowest) {
            lowest = b->board[row][col].answer;
            counter++;
            if (counter > b->clues[clue_index].clue) {
                return false;
            }
        }
        row += b->clues[clue_index].row_d;
        col += b->clues[clue_index].col_d;
    }

    if (counter != b->clues[clue_index].clue)
        return false;

    return true;
}



static bool check_row_legality(board_info_t *b, uint32_t row) {
    uint32_t *checker = calloc(b->ss, sizeof(uint32_t));

    for (uint32_t col = 0; col < b->ss; col++) {
        if (checker[b->board[row][col].answer - 1] > 0) {
            free(checker);
            return false;
        }
        checker[b->board[row][col].answer - 1]++;
    }

    free(checker);
    return true;
}

static bool check_col_legality(board_info_t *b, uint32_t col) {
    uint32_t *checker = calloc(b->ss, sizeof(uint32_t));

    for (uint32_t row = 0; row < b->ss; row++) {
        if (checker[b->board[row][col].answer - 1] > 0) {
            free(checker);
            return false;
        }
        checker[b->board[row][col].answer - 1]++;
    }

    free(checker);
    return true;
}


static bool check_win(board_info_t *b) {
    for (uint32_t row = 0; row < b->ss; row++) {
        if (!check_row_legality(b, row) || !check_col_legality(b, row)) {
            return false;
        }
        for (uint32_t col = 0; col < b->ss; col++) {
            if (b->board[row][col].poss_nb != 1) {
                return false;
            }
        }
    }

    for (uint32_t i = 0; i < b->clue_nb; i++)
        if (!clue_checker(b, i))
            return false;

    return true;
}


static bool guess_next_answer(board_info_t *b, uint32_t poss_guess) {
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


static void copy_board_cells(board_cell_t **orig, board_cell_t **copy, uint32_t ss) {
    for (uint32_t row = 0; row < ss; row++) {
        for (uint32_t col = 0; col < ss; col++) {
            copy[row][col].poss_nb = orig[row][col].poss_nb;
            copy[row][col].answer = orig[row][col].answer;
            for (uint32_t i = 0; i < ss; i++) {
                copy[row][col].poss[i] = orig[row][col].poss[i];
            }
        }
    }
}

static void copy_board_info(board_info_t *orig, board_info_t *copy) {
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


void free_board_info(board_info_t *b) {
    for (uint32_t row = 0; row < b->ss; row++) {
        for (uint32_t col = 0; col < b->ss; col++) {
            free(b->board[row][col].poss);
        }
        free(b->board[row]);
    }
    free(b->board);

    for (uint32_t i = 0; i < b->clue_nb; i++) {
        free(b->clues[i].answers);
    }
    free(b->clues);

    for (uint32_t i = 0; i < b->ss; i++) {
        free(b->row_answers[i].answers);
        free(b->col_answers[i].answers);
    }
    free(b->row_answers);
    free(b->col_answers);
}


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