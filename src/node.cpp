#include "node.h"


//Node::Node(ACTIVATION_FUNCTION f, double w, double b) {
    //linked_nodes = {};
    //switch(f) {
    //case ACTIVATION_FUNCTION::SIGMOID:
        //this->activation = [](double x) -> double {return 1 / (1 + exp(-x));};
        //break;
    //case ACTIVATION_FUNCTION::RELU:
        //this->activation = [](double x) -> double {return (x > 0) ? x : 0;};
        //break;
    //case ACTIVATION_FUNCTION::TANH:
        //this->activation = [](double x) -> double {return tanh(x);};
        //break;
    //}

//}

Node::Node() {
    accumulator = 0.0;
    links = {};
}

void Node::receive_input(double input) {
    accumulator += input;
}

void Node::send_input() const {
    for (auto link: links) {
        link.target->receive_input(accumulator * link.weight);
    }
}

double Node::get_accumulator() const {
    return accumulator;
}

void Node::reset() {
    accumulator = 0.0;
}

void Node::link_to(Node* n, double weight) {
    Link new_link = {n, weight};
    links.push_back(new_link);
}
