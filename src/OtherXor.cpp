#include "OtherXor.h"

OtherXor::OtherXor() : NEAT(2, 2) { }

double operator-(vector<double>& rhs, vector<double>& lhs) {
    double accumulator = 0.0;
    if (rhs.size() != lhs.size())
        throw "vector<double> error: size mismatch";
    for (size_t i=0; i<rhs.size(); i++) {
        accumulator += fabs(rhs.at(i) - lhs.at(i));
    }

    return fabs(accumulator);
}

double OtherXor::play(network_ptr net) {
    auto v0 = net->evaluate_with_actions({0.0, 0.0});
    auto v1 = net->evaluate_with_actions({0.0, 1.0});
    auto v2 = net->evaluate_with_actions({1.0, 0.0});
    auto v3 = net->evaluate_with_actions({1.0, 1.0});

    double fitness = 8.0;
    vector<double> v0_sol = {0.0, 1.0};
    vector<double> v1_sol = {1.0, 0.0};
    vector<double> v2_sol = {1.0, 0.0};
    vector<double> v3_sol = {0.0, 1.0};
    fitness -= v0_sol - v0;
    fitness -= v1_sol - v1;
    fitness -= v2_sol - v2;
    fitness -= v3_sol - v3;
    return fitness;
}
