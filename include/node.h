    #ifndef NODE_H
#define NODE_H

#include <string>
#include <functional>
#include <cmath>
#include <iostream>
#include <vector>

using std::vector;

enum class ACTIVATION_FUNCTION {
    SIGMOID,
    RELU,
    TANH
};

class Node
{
private:
    double w;
    double b;

    double accumulator;
    vector<Node> linked_nodes;
public:
    std::function<double(double)> activation;
    Node();
    Node(ACTIVATION_FUNCTION f, double w, double b);
    static double min(double a, double b);
    static double max(double a, double b);
};

#endif // NODE_H
