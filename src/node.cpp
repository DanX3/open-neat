#include "node.h"

using std::string;

Node::Node(ACTIVATION_FUNCTION f, double w, double b) {
    this->w = w;
    this->b = b;
    linked_nodes = {};
    switch(f) {
    case ACTIVATION_FUNCTION::SIGMOID:
        this->activation = [](double x) -> double {return 1 / (1 + exp(-x));};
        break;
    case ACTIVATION_FUNCTION::RELU:
        this->activation = [](double x) -> double {return Node::max(0, x);};
        break;
    case ACTIVATION_FUNCTION::TANH:
        this->activation = [](double x) -> double {return tanh(x);};
        break;
    }

}

double Node::min(double a, double b) {
    return (a < b) ? a : b;
}

double Node::max(double a, double b) {
    return (a > b) ? a : b;
}

