#include "genome.h"
#include <iostream>


using std::cout;



int main(int argc, char *argv[]) {
    Gene g01 = {0, 0, 1, 1.0};
    Gene g12 = {1, 1, 2, 1.0};
    Gene g13 = {2, 1, 3, 1.0};
    Gene g05 = {4, 0, 5, 1.0, false};
    Gene g45 = {5, 4, 5, 1.0};
    Gene g16 = {6, 1, 6, 1.0};
    Gene g04 = {7, 0, 4, 1.0};
    Genome g1 = {
        {
            g01, g12, g13, g05, g04, g45
        }
    };
    Gene g05_enabled = g05;
    g05_enabled.enabled = true;
    Genome g2 = {
        {
            g01, g16, g12, g05_enabled
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

#if 1
    Genome offspring = g1.crossover(g2);
    offspring.write_to_file("offspring.dot");
#endif
}

