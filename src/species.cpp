#include "species.h"

Species::Species(genome_ptr repr) : representative(repr) {
    genomes = {};
    selectiveness = Config::instance().get_settings().selectiveness;
}

bool Species::is_compatible_with(const Genome& genome) const {
    if (representative->is_compatible(genome))
        return true;
    return false;
}

void Species::add_genome(genome_ptr new_genome) {
    genomes.push_back(new_genome);
};

vector<genome_ptr> Species::get_genomes() const {
    return genomes;
}

bool operator<(genome_ptr lhs, genome_ptr rhs) {
    return (lhs->fitness > rhs->fitness) ? true : false;
}

size_t Species::select_best_genomes() {
    sort(genomes.begin(), genomes.end());
    size_t target_size = (size_t) (selectiveness * genomes.size());
    target_size = (target_size != 0) ? target_size : 1;
    //std::cout << "selectiveness " << selectiveness << '\n';
    //std::cout << "genomes size/ target_size " << genomes.size() << " " << target_size << '\n';
    size_t size_before = genomes.size();
    while (genomes.size() > target_size)
        genomes.pop_back();
    return size_before - genomes.size();
}

ostream& operator<<(ostream& os, const Species& t) {
    os << "Species" << endl
       << "\trepresented by " << &(t.representative) << endl
       << "\tsize " << t.genomes.size() << endl;
    return os;
}

size_t Species::get_size() const {
    return genomes.size();
}
