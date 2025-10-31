#ifndef SKYSCRAPERS_TOOLS_H
#define SKYSCRAPERS_TOOLS_H

#include <stdint.h>
#include <stdbool.h>

#include "skyscrapers_structs.h"

bool clue_checker(board_info_t *b, uint32_t clue_index);

bool check_poss_to_answer(board_info_t *b, uint32_t row, uint32_t col);

uint32_t remove_cell_n_poss(board_info_t *b, uint32_t row, uint32_t col,
                            uint32_t *poss_array, uint32_t n);
int remove_cell_poss(board_info_t *b, uint32_t row, uint32_t col, int possibility);

void elim_answered_poss_col(board_info_t *b, uint32_t col);
void elim_answered_poss_row(board_info_t *b, uint32_t row);

void set_answer(board_info_t *b, uint32_t row, uint32_t col, uint32_t answer);

void print_board(board_info_t *b);

uint32_t factorial(uint32_t nb);

uint32_t is_value_in_array(uint32_t val, uint32_t *array, uint32_t size);

#endif /* SKYSCRAPERS_TOOLS_H */