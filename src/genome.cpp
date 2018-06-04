#include "genome.h"


Genome::Genome(map<size_t, Gene> genes_) {
    genes = genes_;
    node_counter = 0;
    id_counter = 0;
    for (auto pair: genes_) {
        if (pair.second.enabled) {
            activated_genes.insert(pair.second.id);
        }

        // Set node_counter and id_counter 
        if (pair.second.from_link > node_counter)
            node_counter = pair.second.from_link;
        if (pair.second.to_link > node_counter)
            node_counter = pair.second.to_link;
        if (pair.second.id > id_counter)
            id_counter = pair.second.id;
    }

    id_to_layer = {};
    associate_id_to_layer();
    srand(getpid());
    auto last_nodes = find_last_layer_nodes();
    last_layer_node_id = last_nodes;
}

std::ostream& operator<<(std::ostream& os, std::vector<size_t> v) {
    for (auto i: v)
        cout << i << ", ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Genome& g) {
    for (size_t i = 0; i < g.genes.size(); ++i) {
        cout << g.genes.at(i) << endl;
    }
    cout << endl;
    return os;
}

std::vector<size_t> Genome::find_layer0_nodes() const {
    std::vector<size_t> layer0_nodes = {};
    for (auto first: genes) {
        bool valid = true;
        size_t first_start = first.second.from_link;
        for (auto second : genes) {
            if (first_start == second.second.to_link) {
                valid = false;
                break;
            }
        }
        if (valid) {
            bool already_present = false;
            for (auto i: layer0_nodes) {
                if (i == first_start) {
                    already_present = true;
                    break;
                }
            }
            if (not already_present)
                layer0_nodes.push_back(first_start);
        } else
            break;
    }
    return layer0_nodes;
}

set<size_t> Genome::find_last_layer_nodes() const {
    set<size_t> result = {};
    for (auto gene: genes) {
        size_t receiver = gene.second.to_link;
        bool not_a_sender = true;
        for (auto comparison: genes) {
            if (comparison.second.from_link == receiver) {
                not_a_sender = false;
                break;
            }
        }
        if (not_a_sender)
            result.insert(receiver);
    }
    return result;
}



void Genome::associate_id_to_layer() {
    std::vector<size_t> current_focus = find_layer0_nodes();
    id_to_layer.clear();
    for (auto node_id: current_focus)
        id_to_layer.insert({node_id, 0});
    int current_layer = 0;

    // Set the other nodes based on the starting positions found
    std::vector<size_t> next_focus = {};
    do {
        current_layer++;
        for (auto node_id: current_focus) {
            for (auto gene: genes) {
                if (node_id == gene.second.from_link) {
                    size_t target= gene.second.to_link;
                    auto it = id_to_layer.find(target);
                    if (it == id_to_layer.end()) { // if not in id_to_layer
                        id_to_layer.insert({target, current_layer});
                    } else {
                        id_to_layer.at(target) = current_layer;
                    }
                    // TODO: avoid double values in next_focus
                    next_focus.push_back(target);
                }
            }
        }
        current_focus = next_focus;
        next_focus.clear();
    } while(not current_focus.empty());
}

void Genome::write_to_file(std::__cxx11::string filename) const {
    std::ofstream outfile;
    outfile.open(filename);
    outfile << "digraph NN {" << endl;
    for (auto gene_id: activated_genes) {
        outfile << "\t\"" << genes.at(gene_id).from_link
                << "\" -> " << genes.at(gene_id).to_link << endl;
    }
    outfile << "}";
    outfile.close();
}

bool Genome::link_exists(link_t new_link) const {
    link_t reverse_link = {new_link.second, new_link.first};
    for (auto pair: genes) {
        link_t gene_link = {pair.second.from_link, pair.second.to_link};
        if ((gene_link == new_link) or (gene_link == reverse_link)) {
            return true;
        }
    }
    return false;
}

void Genome::mutate_add_link() {
    link_t new_link = {0, 0};
    link_t invalid_link = {0, 0};
    for (auto start: get_rand_seq(0, node_counter)) {
        unsigned short start_layer = id_to_layer.at(start);
        if (last_layer_node_id.find(start) != last_layer_node_id.end())
            continue;
        for (auto i: id_to_layer) {
            if (i.second <= start_layer)
                continue;
            
            if (link_exists({start, i.first}))
                continue;

            new_link.first = start;
            new_link.second = i.first;
            break;
        }
    }
    if (new_link != invalid_link) {
        add_gene(new_link, get_rand_weight());
    }
}

void Genome::mutate_add_node() {
    auto it(activated_genes.begin());
    advance(it, rand() % activated_genes.size());
    Gene disabled_gene = genes.at(*it);
    genes.at(*it).enabled = false;
    activated_genes.erase(it);

    link_t link1(disabled_gene.from_link, ++node_counter);
    link_t link2 = {node_counter, disabled_gene.to_link};
    add_gene(link1, disabled_gene.weight);
    id_counter = add_gene(link2, 1.0);
    associate_id_to_layer();
}


size_t Genome::add_gene(link_t link, double w, bool enabled) {
    Gene new_gene = {++id_counter, link.first, link.second, w};
    genes.insert({new_gene.id, new_gene});
    activated_genes.insert(new_gene.id);
    return new_gene.id;
}


bool Genome::validate_genome() const {
    std::set<size_t> ids = {};
    std::set<string> links = {};
    bool validated = true;
    for (auto pair: genes) {
        // The map id must match the id
        if (pair.first != pair.second.id) {
            std::cerr << "Invalid genome: genes ID must be the same in both index and Gene::id" << endl;
            validated = false;
            break;
        }

        // Check if every id is unique
        auto result = ids.insert(pair.second.id);
        if (! result.second) {
            std::cerr << "Invalid genome: genes were created with the same id" << endl;
            validated = false;
            break;
        }

        // Check if every link is unique
        string link = std::to_string(pair.second.from_link) + "-"
                + std::to_string(pair.second.to_link);
        auto result_link = links.insert(link);
        if (! result_link.second) {
            std::cerr << "Invalid genome: double link found: "
                      << pair.second.from_link  << " -> "
                      << pair.second.to_link << endl;
            validated = false;
            break;
        }
    }

    ids.clear();
    links.clear();
    return validated;
}


//Genome Genome::crossover(Genome other) const {
    
//}

double Genome::get_rand_weight(double scale) const {
    return ((double) rand() / (double)RAND_MAX) * 2.0 * scale - 1.0;
}

std::vector<size_t> Genome::get_rand_seq(size_t min, size_t max_exclusive) {
    std::vector<size_t> result = {};
    result.reserve(max_exclusive - min);
    for (size_t i=0; i<max_exclusive; i++) {
        result.push_back(i);
    }
    for (size_t i=0; i<max_exclusive; i++) {
        std::swap(result[i], result[rand() % result.size()]);
    }
    return result;
}
