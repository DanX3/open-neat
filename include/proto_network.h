#ifndef PROTO_NETWORK_H
#define PROTO_NETWORK_H

#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include <memory>
#include "misc.h"

using std::map;
using std::list;
using std::cout;
using std::endl;
using std::ostream;
using std::shared_ptr;


struct proto_node_t;

typedef shared_ptr<proto_node_t> proto_node_ptr ;

struct proto_node_t {
    size_t id;
    size_t layer;
    list<proto_node_ptr> links;
    proto_node_t(size_t id_, size_t layer_) :
        id(id_), layer(layer_) {
            links = {};
        }
};


class ProtoNetwork {
    private:
        map<size_t, proto_node_ptr> nodes;
        void add_node(size_t id);
        void add_link(size_t from, size_t to);
        void refresh_layers_recursive(proto_node_ptr me, size_t this_layer);
        void refresh_layers();
        list<size_t> layer_0;
    protected:
    public:
        static size_t get_max_edges(size_t vertex);
        ProtoNetwork(list<size_t> layer_0_);
        void add_gene(const gene_t& gene);
        gene_t* mutate_valid_link() const;
        gene_t* mutate_valid_node() const;
        friend ostream& operator<<(ostream& os, const ProtoNetwork& pn);
};

#endif
