#include "genome.h"
#include <iostream>


using std::cout;



int main(int argc, char *argv[]) {
    Genome g = {
        {
            {0, {0, 0, 5, -0.1}},
            {1, {1, 0, 3,  0.2}},
            {2, {2, 5, 6, -0.4}},
            {3, {3, 6, 4, -0.4}},
            {4, {4, 4, 1, -0.4}},
            {5, {5, 4, 2, -0.1}},
            {6, {6, 4, 3, -0.4}},
            {7, {7, 5, 7, -0.4}},
            {8, {8, 7, 4, -0.4}},
        }
    };
    cout << g << std::endl;
    g.write_to_file("before.dot");
    /*
    for (int i=0; i<20; i++) {
        if ((double)rand()/(double)RAND_MAX < 0.5)
            g.mutate_add_node();
        //if ((double)rand()/(double)RAND_MAX < 0.6)
            //g.mutate_add_link();
    }
    g.write_to_file("after.dot");
    */
}

