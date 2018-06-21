#ifndef GENOME_H
#define GENOME_H

#include "misc.h"
#include <map>
#include <memory>
#include "network.h"
#include "proto_network.h"
#include "test.h"

using std::vector;
using std::shared_ptr;
using std::make_shared;

class Genome {
    private:
        map<size_t, private_gene_t> genes;
        shared_ptr<Network> network;
        shared_ptr<ProtoNetwork> proto_net;

        set<size_t> layer_0;
        set<size_t> get_layer_0() const;
        void set_weigth(size_t gene_id, double new_weight);
        void set_enable(size_t gene_id, bool enabled);
        void setup_protonet();
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
       
        //Genome& crossover(const Genome& rhs) const;

};

 #endif
