#ifndef GENOMES_HANDLER_H
#define GENOMES_HANDLER_H

#include <unistd.h>
#include <stack>
#include "id_generator.h"
#include "node_id_generator.h"
#include "id_generator.h"
#include "genome.h"
#include "config.h"
#include "test.h"
#include "species.h"

using std::stack;

class GenomesHandler {
    private:
        int gen_count;
        vector<Species> species;
        stack<genome_ptr> genomes;
        vector<gene_ptr> genes_list;
        vector<gene_ptr> mutated_nodes;
        vector<gene_ptr> mutated_links;
        bool mutate_genome_link(genome_ptr genome);
        void mutate_genome_node(genome_ptr genome);
        bool gene_in(gene_ptr gene, const vector<gene_ptr>& container) const;
        gene_ptr find_gene(size_t from, size_t to,
                const vector<gene_ptr>& container) const;
        double sh(genome_ptr i, genome_ptr j);
        void adjust_fitness();
        void speciate();
        genome_ptr get_genome(size_t i);

    protected:
    public:
        GenomesHandler(size_t input_size, size_t output_size);
        friend ostream& operator<<(ostream& os, const GenomesHandler& gh);
        void mutate_genomes();
        network_ptr get_network(size_t i) const;
};

 #endif
