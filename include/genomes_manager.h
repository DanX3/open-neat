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
#include <string>

using std::stack;
bool operator==(gene_ptr lhs, gene_ptr rhs);
typedef shared_ptr<Genome> genome_ptr;

class GenomesHandler {
    private:
        int gen_count;
        size_t pop_size;
        double delta_t;
        vector<Species> species;
        stack<genome_ptr> genomes;
        set<gene_ptr> genes_list;
        vector<gene_ptr> mutated_nodes;
        vector<gene_ptr> mutated_links;
        bool mutate_genome_link(genome_ptr genome);
        void mutate_genome_node(genome_ptr genome);
        bool gene_in(gene_ptr gene, const vector<gene_ptr>& container) const;
        gene_ptr find_gene(gene_ptr gene, vector<gene_ptr>& container) const;
        gene_ptr find_gene(gene_ptr gene, set<gene_ptr>& container) const;
        double sh(genome_ptr i, genome_ptr j);
        void speciate();
    protected:
    public:
        GenomesHandler(size_t input_size, size_t output_size);
        friend ostream& operator<<(ostream& os, const GenomesHandler& gh);
        void mutate_genomes();
        //void evaluate_genomes();
        void adjust_fitness();
        void reproduce();
        //network_ptr get_network(size_t i) const;
        genome_ptr get_genome(size_t i) const;
        genome_ptr get_best_genome() const;
};

 #endif
