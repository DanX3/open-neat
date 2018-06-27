#ifndef SPECIE_H
#define SPECIE_H

#include "genome.h"

typedef shared_ptr<Genome> genome_ptr;

class Species {
    private:
        vector<genome_ptr> genomes;
        genome_ptr representative;
    protected:
    public:
        Species(genome_ptr repr);
        bool is_compatible_with(const Genome& genome);
        void add_genome(genome_ptr new_genome);
        vector<genome_ptr>& get_genomes();
        void select_best_genomes();
};

 #endif
