#include "proto_network.h"

ProtoNetwork::ProtoNetwork(set<size_t> layer_0_, set<size_t> nodes, 
        set<std::pair<size_t, size_t>> links) :
        ProtoNetwork(layer_0_) {

    for (const auto& i: nodes)
        add_node(i);
    for (const auto& i: links)
        add_link(i.first, i.second);
    refresh_layers();

    layer_n = {};
    for (const auto& i: this->nodes) {
        if (not i.second->has_links())
            layer_n.insert(i.second->id);
    }
}

ProtoNetwork::ProtoNetwork(set<size_t> layer_0_) {
    layer_0 = layer_0_;
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
void ProtoNetwork::add_mutation_link(gene_ptr gene_1) {
    add_node(gene_1->from);
    add_node(gene_1->to);
    add_link(gene_1->from, gene_1->to);
    refresh_layers();
}

/**
 * Applies mutation to the network
 */
void ProtoNetwork::add_mutation_node(gene_ptr gene_1, gene_ptr gene_2) {
    if (gene_1->id > gene_2->id)
        std::swap(gene_1, gene_2);

    add_node(gene_1->to);
    add_link(gene_1->from, gene_1->to);
    add_link(gene_2->from, gene_2->to);
    remove_link(gene_1->from, gene_2->to);
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

        if (start->layer > end->layer)
            std::swap(start, end);

        if (start->layer >= end->layer
                or layer_n.count(start->id) != 0)
                //or layer_n.count(end  ->id) != 0)
            continue;


        bool already_existing = false;
        for (auto i: start->links) {
            if (i->id == end->id) {
                already_existing = true;
                break;
            }
        }
        if (already_existing)
            continue;
        std::cerr << "Adding gene " << start->id << " -> " << end->id << endl;
        return gene_ptr{new gene_t(0, start->id, end->id)};

    }
    return nullptr;
}

/**
 * Return the gene with the link where the new node should be placed
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


shared_ptr<Network> ProtoNetwork::get_network(const map<size_t, private_gene_t> genes) const {
    //refresh_layers();
    shared_ptr<Network> network = make_shared<Network>(get_layers_count());
    for (const auto& node: nodes) {
        network->add_node(node.second->id, node.second->layer);
    }

    for (const auto& node_start: nodes) {
        //auto& node_start = 
        auto start = node_start.second;
        for (const auto& node_end: start->links) {
            auto& end = nodes.at(node_end->id);

            // Look for gene with from->to to get its weight
            double weight;
            for (const auto& g: genes)
                if (g.second.gene.from == start->id and 
                    g.second.gene.to == end->id) {
                    weight = g.second.weight;
                    break;
                }
            network->link_nodes(start->id, start->layer,
                    end->id, end->layer, weight);
        }
    }
    network->finalize();
    return network;
}
