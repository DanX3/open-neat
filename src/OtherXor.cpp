#include "OtherXor.h"

OtherXor::OtherXor() : NEAT(2, 2) { }

double operator-(vector<double>& rhs, vector<double>& lhs) {
    if (rhs.size() != lhs.size()) {
        std::cerr << "Size mismatch " << lhs.size() << " != " << rhs.size() << endl;
        throw "vector<double> error: size mismatch";
    }
    double accumulator = 0.0;
    for (size_t i=0; i<rhs.size(); i++) {
        accumulator += fabs(lhs.at(i) - rhs.at(i));
    }

    //cout << "accumulator = " << accumulator << endl;
    return fabs(accumulator);
}


double OtherXor::play(network_ptr net) {
    auto v0 = net->evaluate_with_actions({2.0, 2.0});
    auto v1 = net->evaluate_with_actions({2.0,  1.0});
    auto v2 = net->evaluate_with_actions({ 1.0, 2.0});
    auto v3 = net->evaluate_with_actions({ 1.0,  1.0});

    double fitness = 0.0;
    vector<double> v0_sol = {0.0, 1.0};
    vector<double> v1_sol = {1.0, 0.0};
    vector<double> v2_sol = {1.0, 0.0};
    vector<double> v3_sol = {0.0, 1.0};
    fitness += v0_sol - v0;
    fitness += v1_sol - v1;
    fitness += v2_sol - v2;
    fitness += v3_sol - v3;
    if (std::isnan(fitness))
        net->write_to_file("nan_network.dot");
        //std::cout << v0 << '\n';
        //std::cout << v1 << '\n';
        //std::cout << v2 << '\n';
        //std::cout << v3 << '\n';
    //}
    return 1.0 - (fitness / 8.0);
}

void OtherXor::showcase(network_ptr net) {
    auto v0 = net->evaluate_with_actions({0.0, 0.0});
    auto v1 = net->evaluate_with_actions({0.0, 1.0});
    auto v2 = net->evaluate_with_actions({1.0, 0.0});
    auto v3 = net->evaluate_with_actions({1.0, 1.0});

    vector<double> v0_sol = {0.0, 1.0};
    vector<double> v1_sol = {1.0, 0.0};
    vector<double> v2_sol = {1.0, 0.0};
    vector<double> v3_sol = {0.0, 1.0};

    cout << "XOR\n"
         << "\t(0, 0) => " << v0 << "(" << v0_sol - v0 << ")" << endl
         << "\t         " << v0_sol << endl
         << "\t(0, 1) => " << v1 << "(" << v1_sol - v1 << ")" << endl
         << "\t         " << v1_sol << endl
         << "\t(1, 0) => " << v2 << "(" << v2_sol - v2 << ")" << endl
         << "\t         " << v2_sol << endl
         << "\t(1, 1) => " << v3 << "(" << v3_sol - v3 << ")" << endl
         << "\t         " << v3_sol << endl;
}

