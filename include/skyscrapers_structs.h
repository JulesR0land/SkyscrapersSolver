#ifndef SKYSCRAPERS_STRUCTS_H
#define SKYSCRAPERS_STRUCTS_H


typedef struct cell_s cell_t;
typedef struct line_info_s line_info_t;
typedef struct skyscrapers_data_s skyscrapers_data_t;


struct cell_s {
    int *poss;         // possibilities
    int poss_nb;       // possibility_number
    int answer;

    line_info_t *parent_row;
    line_info_t *parent_col;
};

struct line_info_s {
    int clue1;
    int clue2;

    cell_t **cells;
};

struct skyscrapers_data_s {
    int size;
    int cell_nb;
    int poss_remaining;

    line_info_t *rows;
    line_info_t *cols;
};

/* OLD
typedef struct line_answers_s {
    int *answers;
    int answer_nb;
} line_answers_t;


typedef struct clues_info_s {
    int clue;

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

*/

#endif /* SKYSCRAPERS_STRUCTS_H */