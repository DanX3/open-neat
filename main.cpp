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
    pnet.add_gene(gene_ptr(new gene_t{0, 0, 4}));
    pnet.add_gene(gene_ptr(new gene_t{1, 0, 3}));
    pnet.add_gene(gene_ptr(new gene_t{2, 4, 1}));
    pnet.add_gene(gene_ptr(new gene_t{3, 2, 3}));
    int counter = 0;
    const int trials = 1000;
    for (int i=0; i<trials; i++) {
        auto result = pnet.mutate_valid_node();
        if (result == nullptr) {
            counter++;
        }
            //std::cout << *result << '\n';
            //std::cout << '*';
    }
    cout << "\nPrecision = " << ((double)trials - (double)counter) / trials * 100.0 << "%\n";
    cout << pnet << endl;
}

