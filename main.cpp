#include "genome.h"
#include <iostream>


using std::cout;



int main(int argc, char *argv[]) {
    Gene g1 = {1, 1, 4, 1.0, true};
    Gene g2 = {2, 2, 4, 1.0, false};
    Gene g3 = {3, 3, 4, 1.0, true};
    Gene g4 = {4, 2, 5, 1.0, true};
    Gene g5 = {5, 5, 4, 1.0, true};
    Gene g6 = {6, 5, 6, 1.0, true};
    Gene g7 = {7, 6, 4, 1.0, true};
    Gene g8 = {8, 1, 5, 1.0, true};
    Gene g9 = {9, 3, 5, 1.0, true};
    Gene g10= {10, 1, 6, 1.0, true};
    Genome G1 = {
        {
            g1, g2, g3, g4, g5, g8
        }
    };

    Genome G2 = {
        {
            g1, g2, g3, g4, {5, 5, 4, 1.0, false}, g6, g7, g9, g10
        }
    };
    cout << g1 << endl;
    cout << g2 << endl;
    //cout << g << std::endl;
#if 0
    g.write_to_file("before.dot");
    for (int i=0; i<40; i++) {
        if ((double)rand()/(double)RAND_MAX < 0.5)
            g.mutate_add_node();
        //if ((double)rand()/(double)RAND_MAX < 0.6)
        g.mutate_add_link();
    }
    g.write_to_file("after.dot");
#endif
    G1.write_to_file("G1.dot");
    G2.write_to_file("G2.dot");
    Genome offspring = G1.crossover(G2);
#if 0
    offspring.mutate_add_node();
    offspring.mutate_add_node();
    offspring.mutate_add_node();
    offspring.mutate_add_node();
    offspring.mutate_add_node();
    offspring.mutate_add_node();
    for (int i=0; i<10; i++) {
        if (i % 2 == 0) {
            offspring.mutate_add_node();
        }
        offspring.mutate_add_link();
    }
#endif
}

