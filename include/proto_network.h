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
#include "node_id_generator.h"

using std::map;
using std::set;
using std::cout;
using std::endl;
using std::ostream;
using std::shared_ptr;
using std::unique_ptr;


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
        void add_node(size_t id);
        void add_link(size_t from, size_t to);
        void remove_link(size_t from, size_t to);
        void refresh_layers_recursive(proto_node_ptr me, size_t this_layer);
        void refresh_layers();
        set<size_t> layer_0;
        size_t recursive_count(const proto_node_t& node, size_t count) const;
    protected:
    public:
        static size_t get_max_edges(size_t vertex);
        ProtoNetwork(set<size_t> layer_0_);
        void add_gene(gene_ptr gene, Mutation m = Mutation::LINK);
        //void add_gene(const gene_t& gene, Mutation m = Mutation::LINK);
        gene_ptr mutate_valid_link() const;
        gene_ptr mutate_valid_node() const;
        void write_to_file(const char* filename) const;
        friend ostream& operator<<(ostream& os, const ProtoNetwork& pn);
        size_t get_layers_count() const;

};

#endif
