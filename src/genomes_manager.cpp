#include "genomes_manager.h"

GenomesHandler::GenomesHandler(size_t input_size, size_t output_size) {
    srand(getpid());
    
    // gene's list initialization
    genes_list = {};
    for (size_t i=0; i<input_size; i++) {
        for (size_t j=0; j<output_size; j++) {
            genes_list.push_back(make_shared<gene_t>(
                        NodeIDGen::instance().get_id(), i, input_size+j));
        }
    }

    // genomes list initialization
    speciate();
    mutated_links = {};
    mutated_nodes = {};
    gen_count = 0;
}

void GenomesHandler::speciate() {
    species = {};
    auto config = Config::instance().get_settings();
    for (size_t i=0; i<config.population_size; i++) {
        genomes.push(make_shared<Genome>(genes_list));
    }

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
    } while (find_gene(new_gene->from, new_gene->to, mutated_links) != nullptr);

    mutated_links.push_back(new_gene);
    gene_ptr new_registered_gene {new gene_t(
            IDGenerator::instance().get_id(), 
            new_gene->from,
            new_gene->to
    )};
    genes_list.push_back(new_registered_gene);
    genome->add_link(new_registered_gene);
    return true;
}

void GenomesHandler::mutate_genome_node(genome_ptr genome) {
    gene_ptr new_gene;
    do { // avoid same mutation in the same generation
        new_gene = genome->mutate_valid_node();
    } while (find_gene(new_gene->from, new_gene->to, mutated_nodes) != nullptr);
    mutated_nodes.push_back(new_gene);
    size_t new_node_id = NodeIDGen::instance().get_id();
    gene_ptr new_registered_gene_from = make_shared<gene_t>(
            IDGenerator::instance().get_id(), 
            new_gene->from, new_node_id);
    gene_ptr new_registered_gene_to = make_shared<gene_t>(
            IDGenerator::instance().get_id(), 
            new_node_id, new_gene->to);
    genes_list.push_back(new_registered_gene_from);
    genes_list.push_back(new_registered_gene_to);
    genome->add_node(new_registered_gene_from, new_registered_gene_to);
}

void GenomesHandler::mutate_genomes() {
    gen_count++;
    mutated_nodes.clear();
    mutated_links.clear();

    auto config = Config::instance().get_settings();
    for (auto& species_i: species) {
        for (const auto& genome: species_i.get_genomes()) {
            if (rand() < config.node_mutation_chance)
                mutate_genome_node(genome);

            if (rand() < config.link_mutation_chance)
                mutate_genome_link(genome);
        }
    }

    // TODO: assign the real fitness after testing
    for (auto& species_i: species) {
        for (const auto& genome: species_i.get_genomes()) {
            genome->get_network()->fitness = (int)(std::pow(-1, rand()%2+1)) 
                * rand() % gen_count;
            cout << genome->get_network()->fitness << endl;
        }
    }
}

gene_ptr GenomesHandler::find_gene(size_t from, size_t to,
        const vector<gene_ptr>& container) const {
    for (const auto& g: container) {
        if (g->from == from and g->to == to)
            return g;
    }
    return nullptr;
}


network_ptr GenomesHandler::get_network(size_t i) const {
    //if (i < genomes.size())
        //return genomes.at(i)->get_network();
    
    for (const auto& species_i: species) {
        if (i < species_i.get_genomes().size())
            return species_i.get_genomes().at(i)->get_network();
        else
            i -= species_i.get_genomes().size();
    }
    return nullptr;
}

double GenomesHandler::sh(genome_ptr i, genome_ptr j) {
    return (i->delta(*j) < Genome::delta_t) ? 1.0 : 0.0;
}

genome_ptr GenomesHandler::get_genome(size_t i) {
    for (const auto& species_i: species) {
        if (i < species_i.get_genomes().size())
            return species_i.get_genomes().at(i);
        else
            i -= species_i.get_genomes().size();
    }
    return nullptr;
}
