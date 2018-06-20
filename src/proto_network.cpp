#include "proto_network.h"

ProtoNetwork::ProtoNetwork(set<size_t> layer_0_) {
    layer_0 = layer_0_;
    srand(time(0));
    nodes = {};
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
}


/**
 * Applies mutation to the network
 */
void ProtoNetwork::add_gene(const gene_ptr gene, Mutation m) {
    add_gene(*gene, m);
}

/**
 * Applies mutation to the network
 */
void ProtoNetwork::add_gene(const gene_t& gene, Mutation m) {
    if (m == Mutation::LINK) {
        add_node(gene.from);
        add_node(gene.to);
        add_link(gene.from, gene.to);
    } else {
        size_t new_node_id = NodeIDGen::instance().get_id();
        add_node(new_node_id);
        add_link(gene.from, new_node_id);
        add_link(new_node_id, gene.to);
        remove_link(gene.from, gene.to);
    }
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

/**
 * Returns the gene representing the new link.
 * The new gene has id = 0 since it still does not know if that will be accepted
 * or not.
 * May return nullptr if the network is already fully connected. Try anyway to
 * avoid this behaviour.
 */
gene_ptr ProtoNetwork::mutate_valid_link() const {
    int trials = 2 * nodes.size();
    while (trials-- > 0) {
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

/**
 * Returns a new gene representing the link from the old to the new node.
 * The new gene has id = 0 since it still does not know if that will be accepted
 * or not.
 * Always returns a valid new gene since is always possible to add a new node to
 * the network.
 */
gene_ptr ProtoNetwork::mutate_valid_node() const {
    gene_ptr mutation = nullptr;
    while (mutation == nullptr) {
        auto it = nodes.begin();
        std::advance(it, rand() % nodes.size());
        proto_node_ptr node = (*it).second;
        if (node->links.size() == 0)
            continue;

        auto links_it = node->links.begin();
        std::advance(links_it, rand() % node->links.size());
        mutation = gene_ptr{new gene_t(0, node->id, (*links_it)->id)};
    }
    return mutation;
}

size_t ProtoNetwork::get_max_edges(size_t n) {
    return n * (n - 1) * 0.5;
}

/**
 * Write the network to a dot file.
 * The image can be generated using i.e. dot -Tsvg output.dot > output.svg
 */
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

size_t ProtoNetwork::get_layers_count() const {
    size_t max = 0;
    for (const auto& i: layer_0) {
        size_t count = recursive_count(*nodes.at(i), 1);
        max = (count > max) ? count : max;
    }
    return max;
}

size_t ProtoNetwork::recursive_count(const proto_node_t& node, size_t layer) const {
    size_t max = layer;
    layer++;
    for (const auto& link: node.links) {
        size_t count = recursive_count(*link, layer);
        max = (count > max) ? count : max;
    }
    return max;
}

