#ifndef PROTO_NETWORK_H
#define PROTO_NETWORK_H

#include <map>
#include <set>
#include <iostream>
#include <algorithm>
#include <memory>
#include <fstream>
#include "misc.h"
#include "test.h"
#include "network.h"

using std::map;
using std::set;
using std::cout;
using std::endl;
using std::ostream;
using std::shared_ptr;
using std::make_shared;


enum class Mutation {
    LINK,
    NODE
};

struct proto_node_t;

typedef shared_ptr<proto_node_t> proto_node_ptr ;


struct proto_node_t {
    size_t id;
    size_t layer;
    set<proto_node_ptr> links;
    proto_node_t(size_t id_, size_t layer_) :
        id(id_), layer(layer_) {
            links = {};
        }
    
    bool has_links() {
        return links.size() != 0;
    }
};

inline ostream& operator<<(ostream& os, const proto_node_t& pn) {
    os << "Proto Node #" << pn.id << " at layer" << pn.layer << endl;
    for (auto link: pn.links) 
        cout << "\t-> " << link->id << endl;
    return os;
}


class ProtoNetwork {
    private:
        map<size_t, proto_node_ptr> nodes;
        set<size_t> layer_0;
        set<size_t> layer_n;
        void add_node(size_t id);
        void add_link(size_t from, size_t to);
        void remove_link(size_t from, size_t to);
        void refresh_layers_recursive(proto_node_ptr me, size_t this_layer);
        void refresh_layers();
        size_t recursive_count(const proto_node_t& node, size_t count) const;
        ProtoNetwork(set<size_t> layer_0_);
    protected:
    public:
        static size_t get_max_edges(size_t vertex);
        ProtoNetwork(set<size_t> layer_0_, set<size_t> nodes, 
                set<std::pair<size_t, size_t>> links);
        void add_mutation_link(gene_ptr gene_1);
        void add_mutation_node(gene_ptr gene_1, gene_ptr gene_2);
        //void add_gene(const gene_t& gene, Mutation m = Mutation::LINK);
        gene_ptr mutate_valid_link() const;
        gene_ptr mutate_valid_node() const;
        void write_to_file(const char* filename) const;
        friend ostream& operator<<(ostream& os, const ProtoNetwork& pn);
        size_t get_layers_count() const;
        shared_ptr<Network> get_network(const map<size_t, private_gene_t> genes) const;

};

#endif
