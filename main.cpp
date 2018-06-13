#include "genomes_manager.h"
#include "misc.h"
//#include "genome.h"
#include <iostream>
#include <vector>
#include "proto_network.h"
using std::vector;
using std::shared_ptr;
//#include "config.h"



int main(int argc, char *argv[]) {
    //GenomesHandler h = {3, 2};
    //cout << h;
    ProtoNetwork pnet = {{0, 2}};
    pnet.add_gene({0, 0, 4});
    pnet.add_gene({1, 0, 3});
    pnet.add_gene({2, 4, 1});
    pnet.add_gene({3, 2, 3});
    for (int i=0; i<100; i++) {
    auto result = pnet.mutate_valid_link();
    if (result == nullptr)
        //std::cout << *result << '\n';
        std::cout << '^';
    else
        std::cout << '*';
    }
    cout << pnet << endl;
}

