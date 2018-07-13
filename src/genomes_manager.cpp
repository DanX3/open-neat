#include "genomes_manager.h"

GenomesHandler::GenomesHandler(size_t input_size, size_t output_size) {
    srand(getpid());
    
    // gene's list initialization
    genes_list = {};
    for (size_t i=0; i<input_size; i++) {
        for (size_t j=0; j<output_size; j++) {
            genes_list.insert(make_shared<gene_t>(
                        IDGenerator::instance().get_id(), i, input_size+j));
        }
    }
    for (const auto& i: genes_list) {
        if (NodeIDGen::instance().check_id() <= i->id)
            NodeIDGen::instance().get_id();
    }

    // genomes list initialization
    mutated_links = {};
    mutated_nodes = {};
    auto config = Config::instance().get_settings();
    pop_size = config.population_size;
    delta_t = config.delta_t;

    // Create first genomes and speciate
    for (size_t i=0; i<pop_size; i++)
        genomes.push(make_shared<Genome>(genes_list));
    species = {};
    speciate();
}

/**
 * Speciation is the action of separating every genome in its species depending
 * on its fitness and structure
 * speciate() takes every genome and assign them in its species
 */
void GenomesHandler::speciate() {
    if (genomes.empty())
        throw "Error: genomes is not empy";

    genomes.top()->write_to_file("genome.dot");
    species = {};

    while (not genomes.empty()) {
        bool inserted = false;
        for (auto& species_i: species) {
            if (species_i.is_compatible_with(*genomes.top())) {
                species_i.add_genome(genomes.top());
                genomes.pop();
                inserted = true;
                break;
            }
        }
        if (not inserted) {
            species.push_back({genomes.top()});
            (*(species.end()-1)).add_genome(genomes.top());
            genomes.pop();
        }
    }
}

ostream& operator<<(ostream& os, const GenomesHandler& gh) {
    //for (auto i: gh.genes_list)
        //cout << *i << endl;
    os << "GenomesHandler" << endl;
    for (const auto& species_i: gh.species) {
        os << species_i << endl;
    }
    return os;
}

bool GenomesHandler::gene_in(gene_ptr gene, const vector<gene_ptr>& container) const {
    for (const auto& gene_i: container) {
        if (*gene == *gene_i)
            return true;
    }
    return false;
}

bool GenomesHandler::mutate_genome_link(genome_ptr genome) {
    gene_ptr new_gene;
    do { // avoid same mutation in the same generation
        new_gene = genome->mutate_valid_link();
        if (new_gene == nullptr) // No new links are possible, quitting...
            return false;
        if (genes_list.find(new_gene) != genes_list.end())
            continue;
    } while (find_gene(new_gene, mutated_links) != nullptr);

    mutated_links.push_back(new_gene);
    gene_ptr new_registered_gene {new gene_t(
            IDGenerator::instance().get_id(), 
            new_gene->from,
            new_gene->to
    )};
    auto result = genes_list.insert(new_registered_gene);
    if (not result.second)
        throw "Gene insertion failed. Not unique";
    genome->add_link(new_registered_gene);
    return true;
}

void GenomesHandler::mutate_genome_node(genome_ptr genome) {
    gene_ptr new_gene;
    size_t trials = genome->get_size();
    do { // avoid same mutation in the same generation
        new_gene = genome->mutate_valid_node();
        if (trials-- <= 0)
            return;
    } while (find_gene(new_gene, mutated_nodes) != nullptr);
    mutated_nodes.push_back(new_gene);
    size_t new_node_id = NodeIDGen::instance().get_id();
    gene_ptr new_registered_gene_from = make_shared<gene_t>(
            IDGenerator::instance().get_id(),
            new_gene->from, new_node_id);
    gene_ptr new_registered_gene_to = make_shared<gene_t>(
            IDGenerator::instance().get_id(),
            new_node_id, new_gene->to);
    genes_list.insert(new_registered_gene_from);
    genes_list.insert(new_registered_gene_to);
    genome->add_node(new_registered_gene_from, new_registered_gene_to);
}

void GenomesHandler::mutate_genomes() {
    mutated_nodes.clear();
    mutated_links.clear();

    auto config = Config::instance().get_settings();
    for (auto& species_i: species) {
        for (auto& genome: species_i.get_genomes()) {
            if (randf() < config.node_mutation_chance)
                mutate_genome_node(genome);

            if (randf() < config.link_mutation_chance)
                mutate_genome_link(genome);
        }
    }
}

//void GenomesHandler::evaluate_genomes() {
    //// TODO: assign the real fitness after testing
    //for (auto& species_i: species) {
        //for (const auto& genome: species_i.get_genomes()) {
            //genome->get_network()->fitness = (int)(std::pow(-1, rand()%2+1))
                //* rand() % gen_count;
            //cout << genome->get_network()->fitness << endl;
        //}
    //}
//}

void GenomesHandler::reproduce() {
    // Selection
    size_t removed = 0;
    for (auto& s: species)
        removed += s.select_best_genomes();

    size_t genomes_left = 0;
    for (const auto& s: species)
        genomes_left += s.get_size();

     //assert(genomes.empty());
    // Reproduction
    while (genomes.size() < pop_size) {
        genomes.push(get_genome(rand() % genomes_left)
                ->crossover(*get_genome(rand() % genomes_left)));
    }
    speciate();
}

gene_ptr GenomesHandler::find_gene(gene_ptr gene,
        set<gene_ptr>& container) const {
    auto result = container.find(gene);
    return (result != container.end()) ? *result: nullptr;
}
gene_ptr GenomesHandler::find_gene(gene_ptr gene,
        vector<gene_ptr>& container) const {
    for (const auto& g: container)
        if (gene == g)
            return g;
    return nullptr;
}


//network_ptr GenomesHandler::get_network(size_t i) const {
    ////if (i < genomes.size())
        ////return genomes.at(i)->get_network();

    //for (const auto& species_i: species) {
        //if (i < species_i.get_size())
            //return species_i.get_genomes().at(i)->get_network();
        //else
            //i -= species_i.get_size();
    //}
    //return nullptr;
//}

double GenomesHandler::sh(genome_ptr i, genome_ptr j) {
    return (i->delta(*j) < Genome::delta_t) ? 1.0 : 0.0;
}

genome_ptr GenomesHandler::get_genome(size_t i) const {
    for (const auto& species_i: species) {
        if (i < species_i.get_size())
            return species_i.get_genomes().at(i);
        else
            i -= species_i.get_size();
    }
    return nullptr;
}

void GenomesHandler::adjust_fitness() {
    genome_ptr src, dest;
    for (size_t i=0; (src = get_genome(i)) != nullptr; i++) {
        //cout << "Genome#" << i
             //<< "fitness " << src->fitness;
             //<< " size: " << src->get_size() << endl;
        double denom = 0.0;
        for (size_t j=0; (dest = get_genome(j)) != nullptr; j++) {
            denom += sh(src, dest);
        }
        src->fitness /= denom;
        //cout << "\tadjusted: " << src->fitness << endl;
    }
}

bool operator==(gene_ptr lhs, gene_ptr rhs) {
    return lhs->from == rhs->from
       and lhs->to   == rhs->to;
}
