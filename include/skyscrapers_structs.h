#ifndef SKYSCRAPERS_STRUCTS_H
#define SKYSCRAPERS_STRUCTS_H

#include <stdint.h>

typedef struct cell_s cell_t;
typedef struct line_info_s line_info_t;
typedef struct skyscrapers_data_s skyscrapers_data_t;


struct cell_s {
    uint32_t *poss;         // possibilities
    uint32_t poss_nb;       // possibility_number
    uint32_t answer;

    line_info_t *parent_row;
    line_info_t *parent_col;
};

struct line_info_s {
    uint32_t clue1;
    uint32_t clue2;

    cell_t **cells;
};

struct skyscrapers_data_s {
    uint32_t size;
    uint32_t cell_nb;
    uint32_t poss_remaining;

    line_info_t *rows;
    line_info_t *cols;
};

/* OLD
typedef struct line_answers_s {
    uint32_t *answers;
    uint32_t answer_nb;
} line_answers_t;


typedef struct clues_info_s {
    uint32_t clue;

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
    uint32_t remaining_poss_nb;

    uint32_t answer_nb;
    uint32_t win;

    board_cell_t **board;
    clues_info_t *clues;

    line_answers_t *row_answers;
    line_answers_t *col_answers;
} board_info_t;

*/

#endif /* SKYSCRAPERS_STRUCTS_H */