#ifndef NODE_H
#define NODE_H

#include <string>
#include <functional>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::string;

struct Link;

enum class ACTIVATION_FUNCTION {
    SIGMOID,
    RELU,
    TANH
};


class Node {
private:
    double accumulator;
    vector<Link> links;
public:
    //std::function<double(double)> activation;
    Node();
    void receive_input(double input);
    void send_input() const;
    double get_accumulator() const;
    void reset();
    void link_to(Node* n, double weight);
    //Node(ACTIVATION_FUNCTION f, double w, double b);
};

struct Link {
    Node* target;
    double weight;
};

#endif
