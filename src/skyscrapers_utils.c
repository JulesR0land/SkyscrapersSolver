#include <stdlib.h>
#include <stdio.h>

#include "skyscrapers_utils.h"
#include "skyscrapers_structs.h"


int *clue_scraper(char *str_clues, int size) {
    int clue_nb = 4 * size;
    int *clues = malloc(sizeof(int) * clue_nb);

    // clue_nb - 1 because there are no ',' for the first clue
    for (int i = 0; i < clue_nb - 1; i++) {
        clues[i] = atoi(str_clues);

        while (*str_clues != ',')
            str_clues++;
        str_clues++;
    }

    clues[clue_nb - 1] = atoi(str_clues);

    return clues;
}


void print_board(skyscrapers_data_t *data) {
    printf("\n");
    for (int col = 0; col < data->size; col++)
        printf("\t   %d", data->cols[col].clue1);

    printf("\n\t");
    for (int i = 0; i < data->size; i++)
        printf("________");
    printf("_\n\n");

    for (int row = 0; row < data->size; row++) {
        printf("%d\t|", data->rows[row].clue1);
        for (int col = 0; col < data->size; col++) {
            for (int poss = 0; poss < data->size; poss++)
                if (data->rows[row].cells[col]->poss[poss] > 0)
                    printf("%d", data->rows[row].cells[col]->poss[poss]);
            printf("\t|");
        }
        printf("\t%d", data->rows[row].clue2);
        printf("\n\t|_______");
        for (int i = 1; i < data->size; i++)
            printf("________");
        printf("|\n\n");
    }

    for (int col = 0; col < data->size; col++)
        printf("\t   %d", data->cols[col].clue2);
    printf("\n");
}


int factorial(int nb) {
    int result = 1;
    for (; nb > 1; nb--)
        result *= nb;

    return result;
}


int is_value_in_array(int val, int *array, int size) {
    for (int i = 0; i < size; i++)
        if (array[i] == val)
            return i;
    return -1;
}