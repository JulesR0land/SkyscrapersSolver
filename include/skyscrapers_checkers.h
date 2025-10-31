#ifndef SKYSCRAPERS_CHECKERS_H
#define SKYSCRAPERS_CHECKERS_H

#include <stdbool.h>

#include "skyscrapers_structs.h"

bool check_row_legality(board_info_t *b, uint32_t row);
bool check_col_legality(board_info_t *b, uint32_t col);

bool check_win(board_info_t *b);


#endif /* SKYSCRAPERS_CHECKERS_H */