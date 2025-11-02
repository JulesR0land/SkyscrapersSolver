#include "skyscrapers_data.h"
#include "skyscrapers_lines.h"
#include "skyscrapers_structs.h"

void free_skyscrapers_data(skyscrapers_data_t *data) {
    free_lines_cells(data->rows, data->size);
    free_lines(data->rows, data->size);
    free_lines(data->cols, data->size);
}


void copy_skyscrapers_data(skyscrapers_data_t *dest, skyscrapers_data_t *data, bool is_dest_initialized) {
    dest->size = data->size;
    dest->cell_nb = data->cell_nb;
    dest->poss_remaining = data->poss_remaining;

    if (is_dest_initialized)
        row_col_copy_data(dest, data);
    else
        row_col_copy_initializer(dest, data);
}
