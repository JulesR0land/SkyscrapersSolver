#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {UP, RIGHT, DOWN, LEFT} direction_t;

typedef struct line_answers_s {
    int *answers;
    int answer_nb;
} line_answers_t;

typedef struct clues_info_s {
    int clue;
    int *answers;
    int answer_nb;

    int row;
    int col;

    int8_t row_d;          // row_direction
    int8_t col_d;          // col_direction
} clues_info_t;

typedef struct board_cell_s {
    int *poss;         // possibilities
    int poss_nb;       // possibility_number
    int answer;
} board_cell_t;

typedef struct board_info_s {
    int ss;            // square_size
    int clue_nb;
    int remaining_poss_nb;

    int answer_nb;
    int win;

    board_cell_t **board;
    clues_info_t *clues;

    line_answers_t *row_answers;
    line_answers_t *col_answers;
} board_info_t;


void elim_answered_poss_col(board_info_t *b, int col);
void elim_answered_poss_row(board_info_t *b, int row);

void set_answer(board_info_t *b, int row, int col, int answer);

int remove_cell_poss(board_info_t *b, int row, int col, int possibility);
int remove_cell_n_poss(board_info_t *b, int row, int col,
                            int *poss_array, int n);

void skyscrapers_solver(int square_size, char *str_clues);




static int factorial(int nb) {
    int result = 1;
    for (; nb > 1; nb--)
        result *= nb;

    return result;
}


static bool line_clue_checker(int *line, int ss, int clue1, int clue2) {
    int counter = 0;
    int lowest = 0;

    for (int i = 0; i < ss; i++) {
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

    for (int i = 0; i < ss; i++) {
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


static int is_value_in_array(int val, int *array, int size) {
    for (int i = 0; i < size; i++)
        if (array[i] == val)
            return i;
    return UINT32_MAX;
}


static void generate_all_comb(const board_cell_t *line, const int line_len,
                              int *current, int **all_comb,
                              const int depth, int *index,
                              int clue1, int clue2) {
    if (depth == line_len) {
        if (!line_clue_checker(current, line_len, clue1, clue2))
            return;

        for (int i = 0; i < line_len; i++) {
            all_comb[*index][i] = current[i];
        }
        (*index)++;
        return;
    }

    for (int i = 0; i < line_len; i++) {
        if (line[depth].poss[i] > 0 &&
            is_value_in_array(line[depth].poss[i], current, depth) == UINT32_MAX) {
            current[depth] = line[depth].poss[i];
            generate_all_comb(line, line_len, current, all_comb, depth + 1, index, clue1, clue2);
        }
    }
}


static void try_possible_row(board_info_t *b, int row) {
    int all_comb_nb = 1;
    int facto = factorial(b->ss);
    int **all_comb = NULL;
    int *current = malloc(sizeof(int) * b->ss);
    board_cell_t *line = malloc(sizeof(board_cell_t) * b->ss);
    int clue1 = b->clues[b->ss * 4 - 1 - row].clue;
    int clue2 = b->clues[b->ss + row].clue;
    int index = 0;

    for (int col = 0; col < b->ss; col++) {
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

    all_comb = malloc(sizeof(int *) * all_comb_nb);
    for (int i = 0; i < all_comb_nb; i++)
        all_comb[i] = calloc(b->ss, sizeof(int));

    generate_all_comb(line, b->ss, current, all_comb, 0, &index, clue1, clue2);

    if (all_comb[0][0] != 0) {
        for (int col = 0; col < b->ss; col++) {
            for (int i = 0; i < b->ss; i++)
                current[i] = i + 1;
            for (int i = 0; all_comb[i][0] != 0 && i < all_comb_nb; i++) {
                current[all_comb[i][col] - 1] = 0;
            }

            remove_cell_n_poss(b, row, col, current, b->ss);
        }
    }


    for (int i = 0; i < all_comb_nb; i++)
        free(all_comb[i]);
    free(all_comb);
    free(line);
}

static void try_possible_col(board_info_t *b, int col) {
    int all_comb_nb = 1;
    int **all_comb = NULL;
    int *current = malloc(sizeof(int) * b->ss);
    board_cell_t *line = malloc(sizeof(board_cell_t) * b->ss);
    int clue1 = b->clues[col].clue;
    int clue2 = b->clues[b->ss * 3 - 1 - col].clue;
    int index = 0;

    for (int row = 0; row < b->ss; row++) {
        line[row] = b->board[row][col];
        all_comb_nb *= b->board[row][col].poss_nb;
    }

    if (all_comb_nb <= 1) {
        free(current);
        free(line);
        return;
    }

    all_comb = malloc(sizeof(int *) * all_comb_nb);
    for (int i = 0; i < all_comb_nb; i++)
        all_comb[i] = calloc(b->ss, sizeof(int));

    generate_all_comb(line, b->ss, current, all_comb, 0, &index, clue1, clue2);

    for (int i = 0; i < b->ss; i++)
        current[i] = 0;

    if (all_comb[0][0] != 0) {
        for (int row = 0; row < b->ss; row++) {
            for (int i = 0; i < b->ss; i++)
                current[i] = i + 1;
            for (int i = 0; all_comb[i][0] != 0 && i < all_comb_nb; i++) {
                current[all_comb[i][row] - 1] = 0;
            }

            remove_cell_n_poss(b, row, col, current, b->ss);
        }
    }

    for (int i = 0; i < all_comb_nb; i++)
        free(all_comb[i]);
    free(all_comb);
    free(line);
}

static void try_possible_lines(board_info_t *b) {
    for (int i = 0; i < b->ss; i++) {
        try_possible_row(b, i);
        try_possible_col(b, i);
    }
}


static void check_alone_poss_row(board_info_t *b, int row) {
    int *counter = calloc(b->ss, sizeof(int));
    int *last_counted = calloc(b->ss, sizeof(int));

    for (int col = 0; col < b->ss; col++)
        if (b->board[row][col].poss_nb > 1)
            for (int i = 0; i < b->ss; i++)
                if (b->board[row][col].poss[i] > 0) {
                    counter[b->board[row][col].poss[i] - 1]++;
                    last_counted[b->board[row][col].poss[i] - 1] = col;
                }

    for (int i = 0; i < b->ss; i++)
        if (counter[i] == 1)
            set_answer(b, row, last_counted[i], i + 1);

    free(counter);
    free(last_counted);
}

static void check_alone_poss_col(board_info_t *b, int col) {
    int *counter = calloc(b->ss, sizeof(int));
    int *last_counted = calloc(b->ss, sizeof(int));

    for (int row = 0; row < b->ss; row++)
        if (b->board[row][col].poss_nb > 1)
            for (int i = 0; i < b->ss; i++)
                if (b->board[row][col].poss[i] > 0) {
                    counter[b->board[row][col].poss[i] - 1]++;
                    last_counted[b->board[row][col].poss[i] - 1] = row;
                }

    for (int i = 0; i < b->ss; i++)
        if (counter[i] == 1)
            set_answer(b, last_counted[i], col, i + 1);

    free(counter);
    free(last_counted);
}

static void check_alone_poss(board_info_t *b) {
    for (int row = 0; row < b->ss; row++) {
        for (int col = 0; col < b->ss; col++) {
            check_alone_poss_row(b, row);
            check_alone_poss_col(b, col);
        }
    }
}


void elim_answered_poss_col(board_info_t *b, int col) {
    for (int row = 0; row < b->ss; row++) {
        for (int i = 0; i < b->col_answers[col].answer_nb; i++)
            remove_cell_poss(b, row, col, b->col_answers[col].answers[i]);
    }
}

void elim_answered_poss_row(board_info_t *b, int row) {
    for (int col = 0; col < b->ss; col++) {
        for (int i = 0; i < b->row_answers[row].answer_nb; i++)
            remove_cell_poss(b, row, col, b->row_answers[row].answers[i]);
    }
}

void set_answer(board_info_t *b, int row, int col, int answer) {
    if (b->board[row][col].answer == answer)
        return;

    for (int i = 0; i < b->ss; i++)
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


static bool check_poss_to_answer(board_info_t *b, int row, int col) {
    if (b->board[row][col].poss_nb != 1)
        return false;

    int i = 0;
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


int remove_cell_n_poss(board_info_t *b, int row, int col,
                                   int *poss_array, int n) {
    int poss_removed = 0;

    for (int i = 0; i < n; i++) {
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

int remove_cell_poss(board_info_t *b, int row, int col, int possibility) {
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
    int to_elim = b->ss - (b->ss - clue->clue + 2) + 1;
    int row = clue->row;
    int col = clue->col;
    int *poss_array = malloc(sizeof(int) * b->ss);

    for (int i = 0; i < b->ss; i++)
        poss_array[i] = b->ss - i;

    for (int i = 0; to_elim > 0; i++) {
        row = clue->row + clue->row_d * i;
        col = clue->col + clue->col_d * i;
        remove_cell_n_poss(b, row, col, poss_array, to_elim);
        to_elim--;
    }

    free(poss_array);
}

static void clue_border_elimination(board_info_t *b) {
    for (int i = 0; i < b->clue_nb; i++) {
        if (b->clues[i].clue > 1) {
            clue_border_elimination_algo(b, &b->clues[i]);
        } else if (b->clues[i].clue == 1) {
            set_answer(b, b->clues[i].row, b->clues[i].col, b->ss);
        }
    }
}


static void line_answers_initializer(board_info_t *b) {
    b->row_answers = malloc(sizeof(line_answers_t) * b->ss);
    b->col_answers = malloc(sizeof(line_answers_t) * b->ss);

    for (int i = 0; i < b->ss; i++) {
        b->row_answers[i].answers = calloc(b->ss, sizeof(int));
        b->row_answers[i].answer_nb = 0;

        b->col_answers[i].answers = calloc(b->ss, sizeof(int));
        b->col_answers[i].answer_nb = 0;
    }
}


static void clue_constructor(clues_info_t *clue_info, int clue,
                             int row, int col,
                             direction_t direction, int ss) {
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

static clues_info_t *clue_parser(int *clues_array, int ss) {
    int clue_nb = 4 * ss;
    clues_info_t *clues = malloc(sizeof(clues_info_t) * clue_nb);

    // clue_nb - 1 because there are no ',' for the first clue
    for (int i = 0; i < clue_nb - 1; i++) {

        if (i < ss)
            clue_constructor(&clues[i], clues_array[i],
                             0, i, DOWN, ss);
        else if (i < 2 * ss)
            clue_constructor(&clues[i], clues_array[i],
                             i - ss, ss - 1, LEFT, ss);
        else if (i < 3 * ss)
            clue_constructor(&clues[i], clues_array[i],
                             ss - 1, (ss - 1) - (i - (2 * ss)), UP, ss);
        else
            clue_constructor(&clues[i], clues_array[i],
                             (ss - 1) - (i - (3 * ss)), 0, RIGHT, ss);
    }

    clue_constructor(&clues[clue_nb - 1], clues_array[clue_nb - 1],
                     (ss - 1) - (clue_nb - 1 - (3 * ss)), 0, RIGHT, ss);

    return clues;
}


static void board_cell_constructor(board_cell_t *cell, int square_size) {
    cell->poss = malloc(sizeof(int) * square_size);

    for (int poss_i = 0; poss_i < square_size; poss_i++)
        cell->poss[poss_i] = poss_i + 1;

    cell->poss_nb = square_size;
    cell->answer = 0;
}

static board_cell_t **board_initializer(int square_size) {
    board_cell_t **board = malloc(sizeof(board_cell_t *) * square_size);

    for (int row = 0; row < square_size; row++) {
        board[row] = malloc(sizeof(board_cell_t) * square_size);
        for (int col = 0; col < square_size; col++) {
            board_cell_constructor(&board[row][col], square_size);
        }
    }

    return board;
}


static bool clue_checker(board_info_t *b, int clue_index) {
    int counter = 0;
    int lowest = 0;
    int row = b->clues[clue_index].row;
    int col = b->clues[clue_index].col;

    if (b->clues[clue_index].clue == 0)
        return true;

    for (int i = 0; i < b->ss; i++) {
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

static bool check_row_legality(board_info_t *b, int row) {
    int *checker = calloc(b->ss, sizeof(int));

    for (int col = 0; col < b->ss; col++) {
        if (checker[b->board[row][col].answer - 1] > 0) {
            free(checker);
            return false;
        }
        checker[b->board[row][col].answer - 1]++;
    }

    free(checker);
    return true;
}

static bool check_col_legality(board_info_t *b, int col) {
    int *checker = calloc(b->ss, sizeof(int));

    for (int row = 0; row < b->ss; row++) {
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
    for (int row = 0; row < b->ss; row++) {
        if (!check_row_legality(b, row) || !check_col_legality(b, row)) {
            return false;
        }
        for (int col = 0; col < b->ss; col++) {
            if (b->board[row][col].poss_nb != 1) {
                return false;
            }
        }
    }

    for (int i = 0; i < b->clue_nb; i++)
        if (!clue_checker(b, i))
            return false;

    return true;
}


static bool guess_next_answer(board_info_t *b, int poss_guess) {
    int row = 0;
    int col = 0;

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

    for (int i = 0; i < b->ss; i++) {
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


static void copy_board_cells(board_cell_t **orig, board_cell_t **copy, int ss) {
    for (int row = 0; row < ss; row++) {
        for (int col = 0; col < ss; col++) {
            copy[row][col].poss_nb = orig[row][col].poss_nb;
            copy[row][col].answer = orig[row][col].answer;
            for (int i = 0; i < ss; i++) {
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

    for (int i = 0; i < orig->ss; i++) {
        copy->row_answers[i].answer_nb = orig->row_answers[i].answer_nb;
        for (int j = 0; j < orig->ss; j++) {
            copy->row_answers[i].answers[j] = orig->row_answers[i].answers[j];
        }
    }

    for (int i = 0; i < orig->ss; i++) {
        copy->col_answers[i].answer_nb = orig->col_answers[i].answer_nb;
        for (int j = 0; j < orig->ss; j++) {
            copy->col_answers[i].answers[j] = orig->col_answers[i].answers[j];
        }
    }
}


bool recursive_it(board_info_t *b) {
    board_info_t copy;
    int current_poss_nb = b->remaining_poss_nb;

    do {
        current_poss_nb = b->remaining_poss_nb;
        try_possible_lines(b);
    } while (current_poss_nb != b->remaining_poss_nb);

    if (b->answer_nb == b->win)
        return check_win(b);

    copy_board_info(b, &copy);
    for (int i = 0; guess_next_answer(b, i); i++) {
        if (recursive_it(b))
            return true;
        copy_board_info(&copy, b);
    }

    return false;
}


void free_board_info(board_info_t *b) {
    for (int row = 0; row < b->ss; row++) {
        for (int col = 0; col < b->ss; col++) {
            free(b->board[row][col].poss);
        }
        free(b->board[row]);
    }
    free(b->board);

    for (int i = 0; i < b->clue_nb; i++) {
        free(b->clues[i].answers);
    }
    free(b->clues);

    for (int i = 0; i < b->ss; i++) {
        free(b->row_answers[i].answers);
        free(b->col_answers[i].answers);
    }
    free(b->row_answers);
    free(b->col_answers);
}


static int **to_int_double_array(board_info_t *b) {
    int **array = malloc(sizeof(int *) * b->ss);

    for (int row = 0; row < b->ss; row++) {
        array[row] = malloc(sizeof(int) * b->ss);
        for (int col = 0; col < b->ss; col++) {
            array[row][col] = b->board[row][col].answer;
        }
    }

    return array;
}

int **SolvePuzzle(int *clues) {
    int **result;
    board_info_t b;

    b.ss = 7;
    b.answer_nb = 0;
    b.win = b.ss * b.ss;
    b.clue_nb = 4 * b.ss;
    b.remaining_poss_nb = b.ss * b.ss * b.ss;
    b.board = board_initializer(b.ss);
    b.clues = clue_parser(clues, b.ss);


    line_answers_initializer(&b);

    clue_border_elimination(&b);

    check_alone_poss(&b);

    if (recursive_it(&b)) {
        result = to_int_double_array(&b);
        free_board_info(&b);
        return result;
    }

    free_board_info(&b);
    return NULL;
}

int main() {

    int clues1[] = {7, 0, 0, 0, 2, 2, 3, 0, 0, 3, 0, 0, 0, 0, 3, 0, 3, 0, 0, 5, 0, 0, 0, 0, 0, 5, 0, 4};
    int clues2[] = {0, 2, 3, 0, 2, 0, 0, 5, 0, 4, 5, 0, 4, 0, 0, 4, 2, 0, 0, 0, 6, 5, 2, 2, 2, 2, 4, 1};

    int **result = SolvePuzzle(clues1);

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

    result = SolvePuzzle(clues2);

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    return 0;
}

/*

1, 5, 6, 7, 4, 3, 2
2, 7, 4, 5, 3, 1, 6
3, 4, 5, 6, 7, 2, 1
4, 6, 3, 1, 2, 7, 5
5, 3, 1, 2, 6, 4, 7
6, 2, 7, 3, 1, 5, 4
7, 1, 2, 4, 5, 6, 3


7, 6, 2, 1, 5, 4, 3
1, 3, 5, 4, 2, 7, 6
6, 5, 4, 7, 3, 2, 1
5, 1, 7, 6, 4, 3, 2
4, 2, 1, 3, 7, 6, 5
3, 7, 6, 2, 1, 5, 4
2, 4, 3, 5, 6, 1, 7

*/