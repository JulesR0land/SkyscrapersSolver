#ifndef SKYSCRAPERS_LINES_H
#define SKYSCRAPERS_LINES_H

#include "skyscrapers_structs.h"

void row_col_initializer(skyscrapers_data_t *data, int *clues);

void row_col_copy_initializer(skyscrapers_data_t *dest, skyscrapers_data_t *data);
void row_col_copy_data(skyscrapers_data_t *dest, skyscrapers_data_t *data);

void free_lines_cells(line_info_t *lines, int size);
void free_lines(line_info_t *lines, int size);

int answer_alone_poss_in_line(line_info_t *line, int size);

void generate_all_comb(const line_info_t *line, const int size,
                       int *current, int **all_comb,
                       const int depth, int *index);

int eliminate_impossible_lines(line_info_t *line, int size);

bool line_legality_checker(int *line, int size, int clue1, int clue2);

#endif /* SKYSCRAPERS_LINES_H */