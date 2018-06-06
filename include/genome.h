#ifndef GENOME_H
#define GENOME_H

//typedef unsigned int size_t;
#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>
#include <unistd.h>
#include <set>
#include <tuple>
#include <iterator>
#include "test.h"
#include "id_generator.h"

using std::cout;
using std::endl;
using std::map;
using std::set;
using std::string;
using std::vector;

//
// GENE
//


struct Gene {
    const size_t id;
    const size_t from_link;
    const size_t to_link;
    double weight;
    bool enabled;

    Gene(size_t id_, size_t from, size_t to,
         double weight_, bool enabled_=true) :
        id(id_), from_link(from), to_link(to), 
        weight(weight_), enabled(enabled_) { }
    //Gene() : Gene(0, 0, 0, 0.0, true) {}
};

inline std::ostream& operator<<(std::ostream& os, const Gene& g) {
    os << "Gene #" << g.id
       << " (enabled: " << g.enabled
       << ", " << g.from_link << " -> " << g.to_link
       << ", weight: " << g.weight
       << ")";
    return os;
}

inline bool operator==(const Gene& lhs, const Gene& rhs) {
    if (lhs.id != rhs.id){
        return false;
    }
    return true;
}

inline bool operator!=(const Gene& lhs, const Gene& rhs) {
    return !(lhs == rhs);
}


//
// GENOME
//

typedef std::pair<size_t, size_t> link_t;

class Genome {
private:
    bool node_counter_changed;
    bool structure_changed;
    map<size_t, Gene> genes;
    set<size_t> activated_genes;
    map<size_t, unsigned short> id_to_layer;
    set<size_t> last_layer_node_id;
    set<size_t> layer0_node_id;
    set<size_t> nodes_id; // Genome node IDs are not sequential, needs tracking
    map<size_t, set<size_t>> links;
    vector<size_t> nodes_id_rand_seq_1;
    vector<size_t> nodes_id_rand_seq_2;


    set<size_t> find_layer0_nodes() const;
    set<size_t> find_last_layer_nodes() const;
    void associate_id_to_layer();
    double get_rand_weight(double scale=1.0) const;
    size_t add_gene(link_t link, double w=1.0, bool enabled=true);
    size_t add_gene(Gene g);
    bool link_exists(link_t new_link) const;
    vector<size_t> get_rand_seq(size_t min, size_t max_exclusive);
    Gene& cross_genes(Gene& lhs, Gene& rhs) const;
    void shuffle_nodes_id_rand_seq();
    void print_links() const; //TODO: delete this
    void insert_link(Gene& gene);
    void remove_link(Gene& gene);


public:
    Genome(vector<Gene> genes_);
    bool validate_genome() const;
    void write_to_file(std::string filename) const;
    friend std::ostream& operator<<(std::ostream& os, const Genome& g);
    void mutate_add_link();
    void mutate_add_node();
    Genome crossover(Genome& other) const;
};

#endif // GENOME_H
