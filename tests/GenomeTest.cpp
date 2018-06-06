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

void bad_genome_constructor() {
    Genome g = {
        {
            {0, 0, 1, -1},
            {0, 0, 2,  1}
        }
    };

}

void validation() {
    Genome g = {
        {
            {0, 0, 1, -0.1},
            {1, 1, 2, -0.1},
            {2, 1, 3, -0.1}
        }
    };

    Genome g2 = {
        {
            {0, 0, 1, -0.1}, 
            {1, 1, 2, -0.1},
            {2, 1, 3, -0.1}
        }
    };

    assert(g.validate_genome());
    assert(g2.validate_genome());
}

void link_existence() {
    Genome possible_link = {
        {
            {0, 0, 1, -0.1},
            {1, 0, 2, -0.1}
        }
    };
    possible_link.mutate_add_link();
    assert(possible_link.validate_genome());

    Genome impossible_link = {
        {
            {0, 0, 1, -0.1},
            {1, 0, 2, -0.1},
            {2, 1, 2, -0.1}
        }
    };
    impossible_link.mutate_add_link();
    assert(impossible_link.validate_genome());
}

void gene_addition() {
    Genome g = {
        {
            {0, 0, 1, 0.1} 
        }
    };
    size_t active_genes = g.activated_genes.size();
    size_t genes_count = g.genes.size();
    g.add_gene({0, 1}, 0.1);
    assert(active_genes == g.activated_genes.size() - 1);
    assert(genes_count == g.genes.size() - 1);
}

void node_mutation() {
    Genome g = {
        {
            {0, 0, 1, 0.1},
            {1, 1, 2, 0.2}
        }
    };
    size_t active_genes = g.activated_genes.size();
    size_t genes_count = g.genes.size();
    g.mutate_add_node();

    assert(active_genes == g.activated_genes.size() - 1);
    assert(genes_count == g.genes.size() - 2);

    for (int i=0; i<10; i++) {
        g.mutate_add_node();
    }

    g.write_to_file("node_mutation.dot");
    assert(g.validate_genome());
}

void heavy_mutation() {
    Genome g = {
        {
            {0, 0, 5, -0.1},
            {1, 0, 3,  0.2},
            {2, 5, 6, -0.4},
            {3, 6, 4, -0.4},
            {4, 4, 1, -0.4},
            {5, 4, 2, -0.1},
            {6, 4, 3, -0.4},
            {7, 5, 7, -0.4},
            {8, 7, 4, -0.4},
        }
    };
    for (int i=0; i<100; i++) {
        if (i % 2 == 0)
            g.mutate_add_node();
        g.mutate_add_link();
    }
    assert(g.validate_genome());
}

void gene_layers_association() {
    Genome g = {
        {
            {0, 0, 5, -0.1},
            {1, 0, 3,  0.2},
            {2, 5, 6, -0.4},
            {3, 6, 4, -0.4},
            {4, 4, 1, -0.4},
            {5, 4, 2, -0.1},
            {6, 4, 3, -0.4},
            {7, 5, 7, -0.4},
            {8, 7, 4, -0.4},
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

void cross_genes_test() {
    Gene g1 = {0, 0, 1, 1.0, true};
    Gene g2 = {0, 0, 1, 1.0, false};
    Genome g = {{}};
    assert(g.cross_genes(g1, g2) == g2);
}

void crossover_test() {
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
    for (int i=0; i<20; i++) {
        if (i % 2 == 0)
            G1.mutate_add_node();
            if (i % 3 != 0)
                G2.mutate_add_node();
        G1.mutate_add_link();
        if (i % 5 != 0)
            G2.mutate_add_link();
    }
    Genome offspring = G1.crossover(G2);
    assert(offspring.validate_genome());
}


int main(int argc, char** argv) {
    try {
        bad_genome_constructor();
        return 1;
    } catch (const char* msg) {}

    try {
        equality();
        inequality();
        validation();
        link_existence();
        gene_addition();
        node_mutation();
        gene_layers_association();
        cross_genes_test();
        for (int j=0; j<5; j++) {
            heavy_mutation();
            crossover_test();
        }
    } catch(const char* e) {
        std::cout << e << '\n';
        return 1;
    }
    return 0;
}
