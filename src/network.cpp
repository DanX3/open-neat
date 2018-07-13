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
    auto actions = evaluate_with_actions(input);
    auto max_el = std::max_element(actions.begin(), actions.end());
    return (max_el - actions.begin());
}

vector<double> Network::evaluate_with_actions(vector<double> input) {
    reset();
    for (size_t i=0; i<net.at(0).size(); i++)
        net.at(0).at(i).receive_input(input.at(i));

    for (auto layer: net)
        for (auto node: layer)
            node.second.send_input();

    auto actions = get_output();
    //for (auto last_node_i: net.at(net.size() - 1))
        //actions.push_back(last_node_i.second.get_accumulator());
    softmax(actions);
    return actions;
}

/**
 * Reset all the nodes' accumulator
 */
void Network::reset() {
    for (size_t i=0; i<net.size(); i++)
        for (auto j=net.at(i).begin(); j!=net.at(i).end(); j++)
            (*j).second.reset();
}

/**
 * Utility to apply a softmax transformation to a vector
 */
void Network::softmax(vector<double>& v) {
    std::for_each(v.begin(), v.end(), [](double& d){ d = std::exp2(d); });
    double sum = 0.0;
    for (const auto& i: v)
        sum += i;
    std::for_each(v.begin(), v.end(), [sum](double& d){ d /= sum; });
}


/**
 * Returns how many layers this network has
 */
size_t Network::get_layers_count() const {
    return net.size();
}

ostream& operator<<(ostream& os, const Network& t) {
    os << "Network" << endl;
    size_t counter = 0;
    for (const auto& layer: t.net) {
        os << "Layer #" << counter++ << " size " << layer.size() << endl;
        os << "\t";
        for (const auto& pair: layer)
            os << pair.first << " ";
        os << endl;
    }
    for (const auto& layer: t.net) {
        for (const auto& pair: layer) {
            cout << pair.second << endl;
        }
        os << endl;
    }
    return os;
}

vector<double> Network::get_output() {
    vector<double> output{};
    for (int i=net.size()-1; i>=0; i--)
        for (const auto& node: net.at(i))
            if (not node.second.has_outgoing_links())
                output.push_back(node.second.get_accumulator());
    return output;
}

void Network::write_to_file(const char* filename) const {
    std::ofstream outfile;
    outfile.open(filename);
    outfile << "digraph NN {" << endl;
    for (const auto& layer: net)
        for (const auto& pair: layer)
            outfile << pair.second.get_id() << " [label=\""
                    << pair.second.get_id() << "("
                    << std::setprecision(3) << pair.second.get_accumulator() 
                    << ")\"]" << endl;

    for (const auto& layer: net)
        for (const auto& pair: layer)
            for (const auto& link: pair.second.get_links())
                outfile << '\t' << pair.second.get_id() << " -> " 
                        << link.target->get_id() << " [label=\""
                        << std::setprecision(3) << link.weight << "\"]" << endl;

    outfile << "}";
    outfile.close();
}
