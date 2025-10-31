#ifndef SKYSCRAPERS_LINES_H
#define SKYSCRAPERS_LINES_H

#include "skyscrapers_structs.h"

void row_col_initializer(skyscrapers_data_t *data, uint32_t *clues);

void row_col_copy_initializer(skyscrapers_data_t *dest, skyscrapers_data_t *data);
void row_col_copy_data(skyscrapers_data_t *dest, skyscrapers_data_t *data);

void free_lines_cells(line_info_t *lines, uint32_t size);
void free_lines(line_info_t *lines, uint32_t size);

#endif /* SKYSCRAPERS_LINES_H */