#include <stdlib.h>

#include "skyscrapers_solver.h"


int main(int ac, char **av) {
    (void)ac;

    int square_size =  atoi(av[1]);

    skyscrapers_solver(square_size, av[2]);
    return 0;
}