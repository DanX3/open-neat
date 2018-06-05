#include "genome.h"
#include <vector>
#include "id_generator.h"
#include <cassert>

void equality() {
    Gene g1 = {0, 0, 1, 0.1};
    Gene g2 = {0, 0, 1, 0.1};
    assert(g1 == g2);
}

void inequality() {
    Gene g1 = {0, 0, 1, 0.1};
    Gene g2 = {1, 0, 1, 0.1};
    assert(g1 != g2);
}


void validation() {
    Genome g = {
        {
            {0, {0, 0, 1, -0.1}},
            {1, {1, 1, 2, -0.1}},
            {2, {2, 1, 3, -0.1}}
        }
    };

    Genome g2 = {
        {
            {1, {0, 0, 1, -0.1}}, // map index must be the same as gene id
            {1, {1, 1, 2, -0.1}},
            {2, {2, 1, 3, -0.1}}
        }
    };

    assert(g.validate_genome());
    assert(g2.validate_genome() == 0);
}

void link_existence() {
    Genome possible_link = {
        {
            {0, {0, 0, 1, -0.1}},
            {1, {1, 0, 2, -0.1}}
        }
    };
    possible_link.mutate_add_link();
    assert(possible_link.validate_genome());

    Genome impossible_link = {
        {
            {0, {0, 0, 1, -0.1}},
            {1, {1, 0, 2, -0.1}},
            {2, {2, 1, 2, -0.1}}
        }
    };
    impossible_link.mutate_add_link();
    assert(impossible_link.validate_genome());
}

void gene_addition() {
    Genome g = {
        {
            {0, {0, 0, 1, 0.1}} 
        }
    };
    size_t active_genes = g.activated_genes.size();
    size_t genes_count = g.genes.size();
    g.add_gene({0, 1}, 0.1);
    assert(g.node_counter == 1);
    assert(g.id_counter == 1);
    assert(active_genes == g.activated_genes.size() - 1);
    assert(genes_count == g.genes.size() - 1);
}

void node_mutation() {
    Genome g = {
        {
            {0, {0, 0, 1, 0.1}},
            {1, {1, 1, 2, 0.2}}
        }
    };
    size_t active_genes = g.activated_genes.size();
    size_t genes_count = g.genes.size();
    g.mutate_add_node();

    assert(g.node_counter == 3);
    assert(g.id_counter == 3);
    assert(active_genes == g.activated_genes.size() - 1);
    assert(genes_count == g.genes.size() - 2);
}

void gene_layers_association() {
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
    assert(g.id_to_layer[0] == 0);
    assert(g.id_to_layer[1] == 4);
    assert(g.id_to_layer[2] == 4);
    assert(g.id_to_layer[3] == 4);
    assert(g.id_to_layer[4] == 3);
    assert(g.id_to_layer[5] == 1);
    assert(g.id_to_layer[6] == 2);
    assert(g.id_to_layer[7] == 2);
}


int main(int argc, char** argv) {
    try {
        equality();
        inequality();
        validation();
        link_existence();
        gene_addition();
        node_mutation();
        gene_layers_association();
    } catch(std::exception e) {
        return 1;
    }
    return 0;
}