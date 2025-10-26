#ifndef SKYSCRAPERS_SOLVER_H
#define SKYSCRAPERS_SOLVER_H

#include <stdint.h>

typedef enum {UP, RIGHT, DOWN, LEFT} direction_t;

typedef struct line_answers_s {
    uint32_t *answers;
    uint32_t answer_nb;
} line_answers_t;

typedef struct clues_info_s {
    uint32_t clue;
    uint32_t *answers;
    uint32_t answer_nb;

    uint32_t row;
    uint32_t col;

    int8_t row_d;          // row_direction
    int8_t col_d;          // col_direction
} clues_info_t;

typedef struct board_cell_s {
    uint32_t *poss;         // possibilities
    uint32_t poss_nb;       // possibility_number
    uint32_t answer;
} board_cell_t;

typedef struct board_info_s {
    uint32_t ss;            // square_size
    uint32_t clue_nb;

    board_cell_t **board;
    clues_info_t *clues;
    line_answers_t *row_answers;
    line_answers_t *col_answers;
} board_info_t;


void elim_answered_poss_col(board_info_t *b, uint32_t col);
void elim_answered_poss_row(board_info_t *b, uint32_t row);

void set_answer(board_info_t *b, uint32_t row, uint32_t col, uint32_t answer);

int remove_cell_poss(board_info_t *b, uint32_t row, uint32_t col, int possibility);

void skyscrapers_solver(uint32_t square_size, char *str_clues);

#endif /* SKYSCRAPERS_SOLVER_H */