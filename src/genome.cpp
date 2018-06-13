#include "genome.h"

const vector<shared_ptr<private_gene_t>>& Genome::get_genes() const {
    return genes;
}

//Genome& Genome::crossover(const Genome& rhs) const {
//}

Genome::Genome() : network{2} {
    genes = {};
}

Genome::Genome(const Genome& src) : network{4} {
    genes = {src.get_genes().begin(), src.get_genes().end()};
    Genome();
}

void Genome::add_gene(const gene_t& g, double weight, bool enabled) {
    genes.push_back(new_private_gene_t(g, weight, enabled));
}
