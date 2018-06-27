#ifndef GENOME_H
#define GENOME_H

#include "misc.h"
#include <map>
#include <memory>
#include "network.h"
#include "proto_network.h"
#include "test.h"
#include "config.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

typedef shared_ptr<Network> network_ptr;

class Genome {
    private:
        map<size_t, private_gene_t> genes;
        network_ptr network;
        shared_ptr<ProtoNetwork> proto_net;

        set<size_t> layer_0;
        set<size_t> get_layer_0() const;
        void set_weigth(size_t gene_id, double new_weight);
        void set_enable(size_t gene_id, bool enabled);
        void setup_protonet();
        Genome(map<size_t, private_gene_t> genes_);
        void insert_iterator(map<size_t, private_gene_t>& genome, 
                map<size_t, private_gene_t>::iterator& it);
    protected:
    public:
        double fitness;
        const map<size_t, private_gene_t>& get_genes() const;
        //Genome(const vector<gene_ptr>& genes_);
        Genome(const vector<gene_ptr>& genes_);
        Genome(const vector<gene_t>& genes_);
        void add_node(gene_ptr to_new_node, gene_ptr from_new_node);
        void add_link(gene_ptr new_link_gene);
        friend ostream& operator<<(ostream& os, const Genome& g);
        gene_ptr mutate_valid_link() const;
        gene_ptr mutate_valid_node() const;
        void mutate_weights();
        shared_ptr<Genome> crossover(const Genome& rhs) ;
        void write_to_file(string filename) const;
        network_ptr get_network();

};

 #endif
