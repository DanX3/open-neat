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
using std::make_unique;

class Genome {
    private:
        map<size_t, private_gene_t> genes;
        unique_ptr<Network> network;
        unique_ptr<ProtoNetwork> proto_net;
        set<size_t> layer_0;
        set<size_t> get_layer_0() const;
    protected:
    public:
        double fitness;
        const map<size_t, private_gene_t>& get_genes() const;
        //Genome(const vector<gene_ptr>& genes_);
        Genome();
        Genome(const vector<gene_t>& genes_);
        Genome(const Genome& src);
        void add_gene(const gene_t& g, double weight, bool enabled);
        void to_string() const;
        void set_weigth(size_t gene_id, double new_weight);
        void set_enable(size_t gene_id, bool enabled);
        gene_ptr mutation_link_proposal() const;
       
        //Genome& crossover(const Genome& rhs) const;

};

 #endif
