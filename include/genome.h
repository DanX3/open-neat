#ifndef GENOME_H
#define GENOME_H

#include "misc.h"
#include <vector>
#include <memory>
#include "network.h"

using std::vector;
using std::shared_ptr;

class Genome {
    private:
        vector<shared_ptr<private_gene_t>> genes;
        Network network;
    protected:
    public:
        double fitness;
        const vector<shared_ptr<private_gene_t>>& get_genes() const;
        Genome();
        Genome(const Genome& src);
        void add_gene(const gene_t& g, double weight, bool enabled);
        //Genome& crossover(const Genome& rhs) const;

};

 #endif
