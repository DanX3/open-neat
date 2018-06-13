#include "network.h"

Network::Network(unsigned short layers_count) {
    editable = true;
    fitness = 0.0;
    for (int i=0; i<layers_count; i++) {
        net.push_back({});
    }
}

/**
 * Makes the network unmodifiable from now on making sure that the network does
 * not change structure during its evaluation
 */
void Network::finalize() {
    editable = false;
}

/**
 * Adds a node to the network.
 * Only a network builder should access this method
 */
void Network::add_node(Node node, unsigned short layer) {
    if (not editable)
        return;
    net.at(layer).insert({node.get_id(), node});
}

/**
 * Creates a connection between nodes
 * Only a network builder should access this method
 * ID node would be enough but this way the complexity to reach a node is
 * reduced from O(Nlog(N)) to O(log(N)) making constant the access to the layer
 */
void Network::link_nodes(size_t node1_id, unsigned short node1_layer, 
                         size_t node2_id, unsigned short node2_layer,
                         double weight) {
    if (not editable)
        return;
    auto node1 = &(*net.at(node1_layer).find(node1_id)).second;
    auto node2 = &(*net.at(node2_layer).find(node2_id)).second;
    node1->link_to(node2, weight);
}

/**
 * Provides an action based on an input. 
 * The input size must match the size of the input layer.
 * No checks are performed anyway so additional values will be ignored but
 * missing values will raise a std::out_of_range exception
 */
int Network::evaluate(vector<double> input) {
    reset();
    for (size_t i=0; i<net.at(0).size(); i++) {
        net.at(0).at(i).receive_input(input.at(i));
    }

    for (auto layer: net) {
        for (auto node: layer) {
            cout << node.second.get_id() << " = " << node.second.get_accumulator() << endl; 
            node.second.send_input();
        }
    }
    
    vector<double> actions = {};
    actions.reserve(net.at(net.size() - 1).size());
    for (auto last_node_i: net.at(net.size() - 1))
        actions.push_back(last_node_i.second.get_accumulator());
    softmax(actions);
    for_each(actions.begin(), actions.end(), [](double d){cout<<d << ' ';});
    auto max_el = std::max_element(actions.begin(), actions.end());
    return (max_el - actions.begin());
}

/**
 * Reset all the nodes' accumulator
 */
void Network::reset() {
    for (auto layer: net)
        for (auto node: layer)
            node.second.reset();
}

/**
 * Utility to apply a softmax transformation to a vector
 */
void Network::softmax(vector<double>& v) {
    std::for_each(v.begin(), v.end(), [](double& d){ d = std::exp(d); });
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    std::for_each(v.begin(), v.end(), [sum](double& d){ d /= sum; });
}


/**
 * Returns how many layers this network has
 */
size_t Network::get_layers_count() const {
    return net.size();
}
