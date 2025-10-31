#include <stdio.h>
#include <stdint.h>

#include "skyscrapers_tools.h"
#include "skyscrapers_checkers.h"


bool clue_checker(board_info_t *b, uint32_t clue_index) {
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


bool check_poss_to_answer(board_info_t *b, uint32_t row, uint32_t col) {
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


void print_board(board_info_t *b) {
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


uint32_t factorial(uint32_t nb) {
    uint32_t result = 1;
    for (; nb > 1; nb--)
        result *= nb;

    return result;
}


uint32_t is_value_in_array(uint32_t val, uint32_t *array, uint32_t size) {
    for (uint32_t i = 0; i < size; i++)
        if (array[i] == val)
            return i;
    return UINT32_MAX;
}