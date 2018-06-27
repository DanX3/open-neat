#include "species.h"

Species::Species(genome_ptr repr) : representative(repr) {
    genomes = {};
}


bool Species::is_compatible_with(const Genome& genome) {
    if (representative->is_compatible(genome))
        return true;
    return false;
}

void Species::add_genome(genome_ptr new_genome) {
    genomes.push_back(new_genome);
};

vector<genome_ptr>& Species::get_genomes() {
    return genomes;
}

void Species::select_best_genomes() {
    sort(genomes.begin(), genomes.end());
    size_t target_size = genomes.size() / 2;
    target_size = (target_size != 0) ? target_size : 1;
    while (genomes.size() > target_size)
        genomes.pop_back();
}
