#ifndef SKYSCRAPERS_SOLVER_H
#define SKYSCRAPERS_SOLVER_H

#include <stdbool.h>

#include "skyscrapers_structs.h"

bool next_guess(skyscrapers_data_t *data, int try_counter);

bool is_win(skyscrapers_data_t *data);

bool recursive_solving(skyscrapers_data_t *data);

void skyscrapers_solver(int square_size, char *str_clues);


#endif /* SKYSCRAPERS_SOLVER_H */