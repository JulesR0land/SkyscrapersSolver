#ifndef SKYSCRAPERS_DATA_H
#define SKYSCRAPERS_DATA_H

#include <stdbool.h>

#include "skyscrapers_structs.h"

void free_skyscrapers_data(skyscrapers_data_t *data);


void copy_skyscrapers_data(skyscrapers_data_t *dest, skyscrapers_data_t *data, bool is_dest_initialized);

#endif /* SKYSCRAPERS_DATA_H */