#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "skyscrapers_checkers.h"
#include "skyscrapers_tools.h"


bool check_row_legality(board_info_t *b, uint32_t row) {
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

bool check_col_legality(board_info_t *b, uint32_t col) {
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


bool check_win(board_info_t *b) {
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