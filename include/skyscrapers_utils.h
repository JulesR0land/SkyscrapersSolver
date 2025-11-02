#ifndef SKYSCRAPERS_UTILS_H
#define SKYSCRAPERS_UTILS_H

#include "skyscrapers_structs.h"


int *clue_scraper(char *str_clues, int size);

void print_board(skyscrapers_data_t *data);

int factorial(int nb);

int is_value_in_array(int val, int *array, int size);

#endif /* SKYSCRAPERS_UTILS_H */