#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void maximum_clique_recursive(const unsigned int **adjmat, unsigned int order,
                                     unsigned int *current_clique, unsigned int *current_clique_size, unsigned int *max_clique,
                                     unsigned int *max_clique_size, unsigned int level)
{
    unsigned int i, connected;
    if (level == order) {
        /* Found a new max clique */
        memcpy(max_clique, current_clique, order * sizeof(unsigned int));
        *max_clique_size = *current_clique_size;
        return;
    }
    /* Find out if current vertex is connected to all vertices in current clique */
    connected = 1;
    for (i = 0; i < level && connected; i++) {
        if (current_clique[i] && !adjmat[level][i]) {
            connected = 0;
        }
    }

    if (connected) {
        /* Add this vertex to the clique */
        current_clique[level] = 1;
        (*current_clique_size)++;
        maximum_clique_recursive(adjmat, order, current_clique, current_clique_size, max_clique,
                                 max_clique_size, level + 1);
        (*current_clique_size)--;
    }
    if (*current_clique_size + order - level > *max_clique_size) {
        /* Still promising */
        current_clique[level] = 0;
        maximum_clique_recursive(adjmat, order, current_clique, current_clique_size, max_clique,
                                 max_clique_size, level + 1);
    }
}

unsigned int maximum_clique(const unsigned int **adjmat, unsigned int order,
                            unsigned int **max_clique)
{
    unsigned int max_clique_size = 0;
    unsigned int current_clique_size = 0;
    unsigned int *current_clique;
    *max_clique;
    if (current_clique == NULL || *max_clique == NULL) {
        free(current_clique);
        free(max_clique);
        return 0;
    }
    maximum_clique_recursive(adjmat, order, current_clique, &current_clique_size, *max_clique,
                             &max_clique_size, 0);
    free(current_clique);
    return max_clique_size;
}



int main()
{
    const unsigned int order = 5; /* Vertices */
    unsigned int r1[] = {0, 1, 0, 1, 1};
    unsigned int r2[] = {1, 0, 1, 0, 1};
    unsigned int r3[] = {0, 1, 0, 0, 1};
    unsigned int r4[] = {1, 0, 0, 0, 1};
    unsigned int r5[] = {1, 1, 1, 1, 0};
    const unsigned int *adjmat[] = {r1, r2, r3, r4, r5};
    unsigned int *max_clique;
    unsigned int max_clique_size = maximum_clique(adjmat, order, &max_clique);
    unsigned int i;
    printf("Max clique size is %u\n", max_clique_size);
    for (i = 0; i < order; i++) {
        if (max_clique[i] == 1) {
            printf("%u ", i);
        }
    }
    putchar('\n');
    free(max_clique);
    return 0;
}