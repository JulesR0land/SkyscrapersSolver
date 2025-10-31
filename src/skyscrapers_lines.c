#include <stdlib.h>

#include "skyscrapers_lines.h"
#include "skyscrapers_cells.h"


void row_col_initializer(skyscrapers_data_t *data, uint32_t *clues) {
    data->rows = malloc(sizeof(line_info_t) * data->size);
    data->cols = malloc(sizeof(line_info_t) * data->size);

    for (uint32_t row = 0; row < data->size; row++) {
        data->rows[row].clue1 = clues[data->size * 4 - 1 - row];
        data->rows[row].clue2 = clues[data->size + row];

        data->rows[row].cells = malloc(sizeof(cell_t *) * data->size);
        for (uint32_t col = 0; col < data->size; col++) {
            data->rows[row].cells[col] = malloc(sizeof(cell_t));
            cell_initializer(data->rows[row].cells[col], data->size);
        }
    }

    for (uint32_t col = 0; col < data->size; col++) {
        data->cols[col].cells = malloc(sizeof(cell_t *) * data->size);

        data->cols[col].clue1 = clues[col];
        data->cols[col].clue2 = clues[data->size * 3 - 1 - col];

        for (uint32_t row = 0; row < data->size; row++) {
            data->cols[col].cells[row] = data->rows[row].cells[col];
            set_parent_lines(data->cols[col].cells[row], &data->rows[row], &data->cols[col]);
        }
    }
}

void row_col_copy_initializer(skyscrapers_data_t *dest, skyscrapers_data_t *data) {
    dest->rows = malloc(sizeof(line_info_t) * dest->size);
    dest->cols = malloc(sizeof(line_info_t) * dest->size);

    for (uint32_t row = 0; row < dest->size; row++) {
        dest->rows[row].clue1 = data->rows[row].clue1;
        dest->rows[row].clue2 = data->rows[row].clue2;

        dest->rows[row].cells = malloc(sizeof(cell_t *) * dest->size);
        for (uint32_t col = 0; col < dest->size; col++) {
            dest->rows[row].cells[col] = malloc(sizeof(cell_t));
            cell_copy_initializer(dest->rows[row].cells[col], data->rows[row].cells[col], dest->size);
        }
    }

    for (uint32_t col = 0; col < dest->size; col++) {
        dest->cols[col].cells = malloc(sizeof(cell_t *) * dest->size);

        dest->cols[col].clue1 = data->cols[col].clue1;
        dest->cols[col].clue2 = data->cols[col].clue2;

        for (uint32_t row = 0; row < dest->size; row++) {
            dest->cols[col].cells[row] = dest->rows[row].cells[col];
            set_parent_lines(dest->cols[col].cells[row], &dest->rows[row], &dest->cols[col]);
        }
    }
}


void row_col_copy_data(skyscrapers_data_t *dest, skyscrapers_data_t *data) {
    for (uint32_t row = 0; row < dest->size; row++) {
        for (uint32_t col = 0; col < dest->size; col++) {
            cell_copy_data(dest->rows[row].cells[col], data->rows[row].cells[col], dest->size);
        }
    }
}


void free_lines_cells(line_info_t *lines, uint32_t size) {
    for (uint32_t i = 0; i < size; i++)
        for (uint32_t j = 0; j < size; j++)
            free_cell(lines[i].cells[j]);
}


void free_lines(line_info_t *lines, uint32_t size) {
    for (uint32_t i = 0; i < size; i++)
        free(lines[i].cells);
    free(lines);
}