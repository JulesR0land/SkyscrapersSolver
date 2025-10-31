#ifndef SKYSCRAPERS_CLUES_H
#define SKYSCRAPERS_CLUES_H

#include "skyscrapers_structs.h"

typedef enum {UP, RIGHT, DOWN, LEFT} direction_t;


void clue_border_elimination_algo(board_info_t *b, clues_info_t *clue);
void clue_border_elimination(board_info_t *b);

void clue_constructor(clues_info_t *clue_info, uint32_t clue,
                      uint32_t row, uint32_t col,
                      direction_t direction);

clues_info_t *clue_parser(char *str_clues, uint32_t ss);


#endif /* SKYSCRAPERS_CLUES_H */