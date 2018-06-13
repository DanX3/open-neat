#include "proto_network.h"

ProtoNetwork::ProtoNetwork(list<size_t> layer_0_) { 
    layer_0 = layer_0_;
    srand(time(0));
    for (auto node: layer_0)
        add_node(node);
}

void ProtoNetwork::add_node(size_t id) {
    auto search_res = nodes.find(id);
    if (search_res == nodes.end()) {
        auto ptr = proto_node_ptr(new proto_node_t(id, 0));
        nodes.insert(std::pair<size_t, proto_node_ptr>(id, ptr));
    }
}

void ProtoNetwork::add_link(size_t from, size_t to) {
    nodes.at(from)->links.push_back(nodes.at(to));
}

void ProtoNetwork::add_gene(const gene_t& gene) {
    add_node(gene.from);
    add_node(gene.to);
    add_link(gene.from, gene.to);
    refresh_layers();
}

ostream& operator<<(ostream& os, const ProtoNetwork& pn) {
    for (auto i: pn.nodes) {
        os << "Node #" << i.second->id << " at " << i.second->layer << endl;
        for (auto link: i.second->links)
            os << "\t" << "-> " << link->id << endl;
    }
    return os;
}

void ProtoNetwork::refresh_layers() {
    for (size_t node: layer_0)
        refresh_layers_recursive(nodes.at(node), 0);

}

void ProtoNetwork:: refresh_layers_recursive(proto_node_ptr me, size_t this_layer) {
    if (this_layer < me->layer)
        return;

    me->layer = this_layer;
    size_t children_layer = this_layer + 1;
    for (auto child: me->links)
        refresh_layers_recursive(child, children_layer);
}

gene_t* ProtoNetwork::mutate_valid_link() const {
    size_t max_trials = get_max_edges(nodes.size());
    while (max_trials > 0) {
        max_trials--;
        auto start_it = nodes.begin();
        auto end_it = nodes.begin();

        std::advance(start_it, rand() % nodes.size());
        std::advance(end_it, rand() % nodes.size());

        proto_node_ptr start, end;
        start = (*start_it).second;
        end = (*end_it).second;

        if (start->layer == end->layer)
            continue;

        if (start->layer >= end->layer)
            std::swap(start, end);

        auto it = std::find(start->links.begin(), start->links.end(), end);
        if (it != start->links.end())
            continue;

        return new gene_t{0, start->id, end->id};

    }
    return nullptr;
}

size_t ProtoNetwork::get_max_edges(size_t n) {
    return n * (n - 1) * 0.5;
}
