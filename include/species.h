#ifndef SPECIE_H
#define SPECIE_H

#include "genome.h"

typedef shared_ptr<Genome> genome_ptr;

class Species {
    private:
        double selectiveness;
        vector<genome_ptr> genomes;
        genome_ptr representative;
    protected:
    public:
        Species(genome_ptr repr);
        bool is_compatible_with(const Genome& genome) const;
        void add_genome(genome_ptr new_genome);
        vector<genome_ptr> get_genomes() const;
        size_t get_size() const;
        size_t select_best_genomes();
        friend ostream& operator<<(ostream& os, const Species& t);
};

bool operator<(genome_ptr lhs, genome_ptr rhs);

 #endif
