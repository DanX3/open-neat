#include "genome.h"

Genome::Genome(std::vector<Gene> genes_) {
    genes = {};
    nodes_id = {};
    links = {};
    nodes_id_rand_seq_1 = {};
    nodes_id_rand_seq_2 = {};
    node_counter_changed = true;
    structure_changed = true;
    for (auto gene: genes_) {
        auto out = genes.insert({gene.id, gene});
        if (not out.second) {
            throw "Not unique gene ID exception";
        }
    }

    // Skipping used gene's id
    for (auto pair: genes) {
        while (IDGenerator::instance().check_id() <= pair.second.id) {
            IDGenerator::instance().get_id();
        }
    }

    for (auto gene: genes_) {
        if (gene.enabled) {
            activated_genes.insert(gene.id);
            insert_link(gene);
        }
    }

    id_to_layer = {};
    layer0_node_id = find_layer0_nodes();
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
    //if (not structure_changed)
        //return;

    structure_changed = false;
    set<size_t> current_focus = layer0_node_id;
    id_to_layer.clear();
    for (auto node_id: current_focus)
        id_to_layer.insert({node_id, 0});
    int current_layer = 0;
    set<size_t> next_focus = {};

    do {
        current_layer++;
        for (auto node_id: current_focus) {
            if (links.find(node_id) == links.end())
                continue;
            for (auto target: links.at(node_id)) {
                auto it = id_to_layer.find(target);
                if (it == id_to_layer.end()) { // if not in id_to_layer
                    id_to_layer.insert({target, current_layer});
                } else {
                    id_to_layer.at(target) = current_layer;
                }
                next_focus.insert(target);
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
        outfile << "\t" << genes.at(gene_id).from_link << " -> " 
                << genes.at(gene_id).to_link << endl;
    }
    outfile << "}";
    outfile.close();
}

/**
 * Check if the link exists already or not. 
 * WARNING
 * Does not check the link validity, that is from a higher layer to a higher one
 * (i.e. if a link starts from layer 3 and targets a node in layer 2). This is
 * an easy checking that can be performed through id_to_layer data structure
 *
 */
bool Genome::link_exists(link_t new_link) const {
    if (links.find(new_link.first) == links.end())
        return false;
    auto link = links.at(new_link.first);
    if (link.find(new_link.second) == link.end())
        return false;
    return true;
}

/**
 * Adds a link to the genome keeping the network structure as a DAG. The weight
 * of the new connection is in [-1.0, 1.0]
 */
void Genome::mutate_add_link() {
    link_t new_link = {0, 0};
    link_t invalid_link = {0, 0};
    shuffle_nodes_id_rand_seq();
    for (auto start: nodes_id_rand_seq_1) {
        unsigned short start_layer = id_to_layer.at(start);
        if (last_layer_node_id.find(start) != last_layer_node_id.end())
            continue;
        for (auto end: nodes_id_rand_seq_2) {
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
    // Deletion phase
    auto it = activated_genes.begin();
    advance(it, rand() % activated_genes.size());
    Gene disabled_gene = genes.at(*it);
    genes.at(*it).enabled = false;
    remove_link(disabled_gene);
    activated_genes.erase(it);

    // Creation phase
    size_t new_node_id = IDGenerator::instance().get_id();
    nodes_id.insert(new_node_id);
    link_t link1 = {disabled_gene.from_link, new_node_id};
    link_t link2 = {new_node_id, disabled_gene.to_link};
    structure_changed = true;
    add_gene(link1, disabled_gene.weight);
    structure_changed = true;
    add_gene(link2, 1.0);
}


size_t Genome::add_gene(link_t link, double w, bool enabled) {
    Gene new_gene = {IDGenerator::instance().get_id(), 
        link.first, link.second, w, true};
    return add_gene(new_gene);
}

size_t Genome::add_gene(Gene new_gene) {
    genes.insert({new_gene.id, new_gene});
    insert_link(new_gene);
    if (new_gene.enabled) {
        activated_genes.insert(new_gene.id);

    }
    associate_id_to_layer();
    return new_gene.id;
}

/**
 * Perform all the checking to validate a genome structure as a DAG.
 * Mainly used for tesing purposes
 */
bool Genome::validate_genome() const {
    std::set<size_t> ids = {};
    bool validated = true;
    for (auto pair: genes) {
        // Check if every id is unique
        auto result = ids.insert(pair.second.id);
        if (! result.second) {
            std::cerr << "Invalid genome: genes were created with the same id" << endl;
            validated = false;
            break;
        }
    }

    // Check if every link is unique
    auto links_copy = links;
    for (auto pair: genes) {
        if (not pair.second.enabled)
            continue;
        Gene gene = pair.second;
        links_copy.at(gene.from_link).erase(gene.to_link);
    }
    for (auto pair: links_copy) {
        if (pair.second.size() != 0) {
            std::cerr << "Double link found: " << pair.first << " -> " 
                 << *pair.second.begin() << endl;
            validated = false;
            break;
        }
    }

    // Check if every link is to a higher layer node
    for (auto pair: genes) {
        try {
            size_t starting_layer = id_to_layer.at(pair.second.from_link);
            size_t ending_layer = id_to_layer.at(pair.second.to_link);
            if (ending_layer <= starting_layer) {
                validated = false;
                break;
            }
        } catch (const char* msg) {
            std::cerr << "id_to_layer.size() = " << id_to_layer.size() << endl;
            ids.clear();
            links_copy.clear();
            return false;
        }
    }

    ids.clear();
    links_copy.clear();
    return validated;
}


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
    vector<Gene> result;
    auto myit = genes.begin();
    auto otherit = other.genes.begin();
    while (true) {
        Gene mygene = myit->second;
        Gene othergene = otherit->second;

        if (myit == genes.end()) {
            while (otherit != other.genes.end()) {
                result.push_back(otherit->second);
                std::advance(otherit, 1);
            }
            break;
        }

        if (otherit == other.genes.end()) {
            while (myit != genes.end()) {
                result.push_back(myit->second);
                std::advance(myit, 1);
            }
            break;
        }

        if (mygene.id == othergene.id) {
            Gene valid_gene = cross_genes(mygene, othergene);
            result.push_back(valid_gene);
            std::advance(myit, 1);
            std::advance(otherit, 1);
            continue;
        }
        
        if (mygene.id < othergene.id) {
            result.push_back(mygene);
            std::advance(myit, 1);
            continue;
        }

        if (mygene.id > othergene.id) {
            result.push_back(othergene);
            std::advance(otherit, 1);
            continue;
        }
    }
    Genome offspring = {result};
    return offspring;
}

void Genome::shuffle_nodes_id_rand_seq() {
    if (node_counter_changed) {
        node_counter_changed = false;
        nodes_id_rand_seq_1.clear();
        nodes_id_rand_seq_2.clear();
        for (size_t node_id: nodes_id) {
            nodes_id_rand_seq_1.push_back(node_id);
            nodes_id_rand_seq_2.push_back(node_id);
        }
    }
    const size_t size = nodes_id_rand_seq_1.size();
    for (size_t i=0; i<nodes_id_rand_seq_1.size() / 2; i++) {
        std::swap(nodes_id_rand_seq_1[i], nodes_id_rand_seq_1[rand() % size]);
        std::swap(nodes_id_rand_seq_2[i], nodes_id_rand_seq_2[rand() % size]);
    }
}

void Genome::print_links() const {
    for (auto start: links) {
        cout << start.first << " -> ";
        for (auto end: start.second) {
            cout << end << ", ";
        }
        cout << endl;
    }
}

void Genome::insert_link(Gene& gene) {
    if (not gene.enabled)
        return;

    auto out = links.find(gene.from_link);
    if (out == links.end()) { // starting node still not present
        links.insert({gene.from_link, {gene.to_link}});
    } else {  // Starting node present but END is missing
        out->second.insert(gene.to_link);
    }
}

void Genome::remove_link(Gene& gene) {
    auto out = links.find(gene.from_link);
    out->second.erase(gene.to_link);
}
