#include "proto_network.h"

ProtoNetwork::ProtoNetwork(set<size_t> layer_0_) {
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
    while (NodeIDGen::instance().check_id() <= id)
        NodeIDGen::instance().get_id();
}

void ProtoNetwork::add_link(size_t from, size_t to) {
    nodes.at(from)->links.insert(nodes.at(to));
}

void ProtoNetwork::remove_link(size_t from, size_t to) {
    set<proto_node_ptr>* links = &nodes.at(from)->links;
    auto result = links->find(nodes.at(to));
    nodes.at(from)->links.erase(result);
    cout << *nodes.at(from) << endl;
}

void ProtoNetwork::add_gene(gene_ptr gene, Mutation m) {
    if (gene == nullptr)
        return;

    if (m == Mutation::LINK) {
        add_node(gene->from);
        add_node(gene->to);
        add_link(gene->from, gene->to);
        refresh_layers();
    } else {
        cout << "Adding new node " << gene->from << " -> " << gene->to << endl;
        size_t new_node_id = NodeIDGen::instance().get_id();
        add_node(new_node_id);
        add_link(gene->from, new_node_id);
        add_link(new_node_id, gene->to);
        cout << "size before " << nodes.at(gene->from)->links.size() << endl;
        remove_link(gene->from, gene->to);
        cout << "size after " << nodes.at(gene->from)->links.size() << endl;
    }
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

gene_ptr ProtoNetwork::mutate_valid_link() const {
    size_t max_trials = get_max_edges(nodes.size());
    while (max_trials-- > 0) {
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

        return gene_ptr{new gene_t(0, start->id, end->id)};

    }
    return nullptr;
}

gene_ptr ProtoNetwork::mutate_valid_node() const {
    size_t trials = nodes.size();
    while (trials-- > 0) {
        auto it = nodes.begin();
        std::advance(it, rand() % nodes.size());
        proto_node_ptr node = (*it).second;
        if (node->links.size() == 0)
            continue;

        auto links_it = node->links.begin();
        std::advance(links_it, rand() % node->links.size());
        return gene_ptr{new gene_t(0, node->id, (*links_it)->id)};
    }
    return nullptr;
}

size_t ProtoNetwork::get_max_edges(size_t n) {
    return n * (n - 1) * 0.5;
}

void ProtoNetwork::write_to_file(const char* filename) const {
    std::ofstream outfile;
    outfile.open(filename);
    outfile << "digraph NN {" << endl;
    for (auto node: nodes) {
        for (auto link: node.second->links) {
            outfile << "\t"
                    << node.second->id << " -> "
                    << link->id << endl;
        }
    }
    outfile << "}";
    outfile.close();
}
