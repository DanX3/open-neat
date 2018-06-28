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

void Species::select_best_genomes() {
    sort(genomes.begin(), genomes.end());
    cout << endl;
    size_t target_size = (size_t) (selectiveness * genomes.size());
    target_size = (target_size != 0) ? target_size : 1;
    while (genomes.size() > target_size)
        genomes.pop_back();
}

ostream& operator<<(ostream& os, const Species& t) {
    os << "Species" << endl
       << "\trepresented by " << &(t.representative) << endl
       << "\tsize " << t.genomes.size() << endl;
    return os;
}
