#include "genomes_manager.h"

GenomesHandler::GenomesHandler(size_t input_size, size_t output_size) {
    auto config = Config::instance().get_settings();
    
    // gene's list initialization
    genes_list = {};
    for (size_t i=0; i<input_size; i++) {
        for (size_t j=0; j<output_size; j++) {
            genes_list.push_back(make_shared<gene_t>(
                        NodeIDGen::instance().get_id(), i, input_size+j));
        }
    }

    // genomes list initialization
    genomes = {};
    for (size_t i=0; i<config.population_size; i++) {
        genomes.push_back(make_shared<Genome>(genes_list));
    }
    cout << "Created " << genomes.size() << " genomes\n";
    mutated_links = {};
    mutated_nodes = {};
}

ostream& operator<<(ostream& os, const GenomesHandler& gh) {
    for (auto i: gh.genes_list)
        cout << *i << endl;
    return os;
}

bool GenomesHandler::gene_in(gene_ptr gene, const vector<gene_ptr>& container) const {
    for (const auto& gene_i: container) {
        if (*gene == *gene_i)
            return true;
    }
    return false;
}

void GenomesHandler::mutate_genome_link(genome_ptr genome) {
    std::cerr << "Genome to mutate\n" << *genome << endl;
    gene_ptr new_gene;
    do {
        new_gene = genome->mutate_valid_link();
        if (new_gene == nullptr)
            return;
    } while (not gene_in(new_gene, mutated_links));
    mutated_links.push_back(new_gene);
    gene_ptr new_registered_gene {new gene_t(
            IDGenerator::instance().get_id(), 
            new_gene->from,
            new_gene->to
    )};
    genes_list.push_back(new_registered_gene);
}

void GenomesHandler::mutate_genome_node(genome_ptr genome) {
}

void GenomesHandler::mutate_genomes() {
    auto config = Config::instance().get_settings();
    for (auto& genome: genomes) {
        if (rand() < config.node_mutation_chance)
            mutate_genome_node(genome);

        if (rand() < config.link_mutation_chance)
            mutate_genome_link(genome);
    }
}
