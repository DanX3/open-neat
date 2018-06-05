#include "genome.h"


Genome::Genome(std::vector<Gene> genes_) {
    genes = {};
    nodes_id = {};
    for (auto gene: genes_) {
        auto out = genes.insert({gene.id, gene});
        if (not out.second) {
            throw "Not unique gene ID exception";
        }
    }
    node_counter = 0;
    id_counter = 0;
    for (auto gene: genes_) {
        if (gene.enabled) {
            activated_genes.insert(gene.id);
        }

        // Set node_counter and id_counter 
        if (gene.from_link > node_counter)
            node_counter = gene.from_link;
        if (gene.to_link > node_counter)
            node_counter = gene.to_link;
        if (gene.id > id_counter)
            id_counter = gene.id;
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
    for (auto pair: g.genes){
        cout << pair.second << endl;
    }
    cout << endl;
    return os;
}

std::set<size_t> Genome::find_layer0_nodes() const {
    set<size_t> layer0_nodes = {};
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
            layer0_nodes.insert(first_start);
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
    set<size_t> current_focus = find_layer0_nodes();
    id_to_layer.clear();
    for (auto node_id: current_focus)
        id_to_layer.insert({node_id, 0});
    int current_layer = 0;

    // Set the other nodes based on the starting positions found
    set<size_t> next_focus = {};
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
                    next_focus.insert(target);
                }
            }
        }
        // Just print
        write_to_file("dump");
        for (auto i: next_focus)
            cout << i << " - " ;
        cout << endl;

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

/**
 * Adds a link to the genome keeping the network structure as a DAG. The weight
 * of the new connection is in [-1.0, 1.0]
 */
void Genome::mutate_add_link() {
    link_t new_link = {0, 0};
    link_t invalid_link = {0, 0};
    for (auto start: get_nodes_id_rand_seq()) {
        unsigned short start_layer = id_to_layer.at(start);
        if (last_layer_node_id.find(start) != last_layer_node_id.end())
            continue;
        for (auto end: get_nodes_id_rand_seq()) {
            if (id_to_layer[end] <= start_layer)
                continue;
            
            if (link_exists({start, end}))
                continue;

            new_link.first = start;
            new_link.second = end;
            break;
        }
    }
    if (new_link != invalid_link) {
        add_gene(new_link, get_rand_weight());
    }
}

/*
 * Adds a node following the rules of NEAT, splitting a connection, setting the
 * connection to the new node with the old weight and the connection from the
 * new node with a weight of 1.0
 */
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
    Gene new_gene = {++id_counter, link.first, link.second, w, true};
    return add_gene(new_gene);
}

size_t Genome::add_gene(Gene new_gene) {
    genes.insert({new_gene.id, new_gene});
    if (new_gene.enabled)
        activated_genes.insert(new_gene.id);
    nodes_id.insert(new_gene.from_link);
    nodes_id.insert(new_gene.from_link);
    return new_gene.id;
}

/**
 * Perform all the checking to validate a genome structure as a DAG.
 * Mainly used for tesing purposes
 */
bool Genome::validate_genome() const {
    std::set<size_t> ids = {};
    std::set<string> links = {};
    bool validated = true;
    for (auto pair: genes) {
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

        // Check if every link is to a higher layer node
        for (auto pair: genes) {
            size_t starting_layer = id_to_layer.at(pair.second.from_link);
            size_t ending_layer = id_to_layer.at(pair.second.to_link);
            if (ending_layer <= starting_layer) {
                validated = false;
                break;
            }
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

/**
 * Utility function that performs crossover between genes
 * If a gene is disabled, disables also the other one
 */
Gene& Genome::cross_genes(Gene& lhs, Gene& rhs) const {
    if (lhs.enabled == rhs.enabled)
        return lhs;
    return lhs.enabled ? rhs : lhs;
}


/**
 * Performs crossover between two genomes returning the new one
 * The genome so created has the genes from both genomes resulting in a new
 * structure
 */
Genome Genome::crossover(Genome& other) const {
    Genome result = {{}};
    auto myit = genes.begin();
    auto otherit = other.genes.begin();
    std::cout << id_counter << '\n';
    std::cout << other.id_counter << '\n';
    while (true) {
        Gene mygene = myit->second;
        Gene othergene = otherit->second;

        if (myit == genes.end()) {
            while (otherit != other.genes.end()) {
                result.add_gene(otherit->second);
                std::advance(otherit, 1);
            }
            break;
        }

        if (otherit == other.genes.end()) {
            while (myit != genes.end()) {
                result.add_gene(myit->second);
                std::advance(myit, 1);
            }
            break;
        }

        if (mygene.id == othergene.id) {
            Gene valid_gene = cross_genes(mygene, othergene);
            result.add_gene(valid_gene);
            std::advance(myit, 1);
            std::advance(otherit, 1);
            continue;
        }
        
        if (mygene.id < othergene.id) {
            result.add_gene(mygene);
            std::advance(myit, 1);
            continue;
        }

        if (mygene.id > othergene.id) {
            result.add_gene(othergene);
            std::advance(otherit, 1);
            continue;
        }
    }
    result.write_to_file("offspring.dot");
    cout << result;
    return result;
}

vector<size_t> Genome::get_nodes_id_rand_seq() const {
    vector<size_t> result = {nodes_id.begin(), nodes_id.end()};
    for (size_t i=0; i<result.size(); i++) {
        std::swap(result[i], result[rand() % result.size()]);
    }
    return result;
}
