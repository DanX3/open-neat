#include "genome.h"
#include <iostream>


using std::cout;



int main(int argc, char *argv[]) {
    Genome g = {
        {
             {0, {0, 0, 1, -0.1}}
            ,{1, {1, 0, 2, -0.4}}
        }
    };
    //cout << g << std::endl;
#if 1
    g.write_to_file("before.dot");
    for (int i=0; i<40; i++) {
        if ((double)rand()/(double)RAND_MAX < 0.5)
            g.mutate_add_node();
        //if ((double)rand()/(double)RAND_MAX < 0.6)
        g.mutate_add_link();
    }
    g.write_to_file("after.dot");
#endif
}

