#ifndef SKYSCRAPERS_SOLVER_H
#define SKYSCRAPERS_SOLVER_H

#include <stdbool.h>

#include "skyscrapers_structs.h"

void print_board(skyscrapers_data_t *data);
void skyscrapers_solver(int square_size, char *str_clues);


#endif /* SKYSCRAPERS_SOLVER_H */