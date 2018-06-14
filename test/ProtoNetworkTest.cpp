#include "proto_network.h"
#include <cassert>
#include <vector>

using std::vector;


ProtoNetwork create_network() {
    ProtoNetwork net = {{0, 2}};
    net.add_gene({0, 0, 4});
    net.add_gene({1, 0, 3});
    net.add_gene({2, 4, 1});
    net.add_gene({3, 2, 3});
    return net;
}

void test_get_max_edges() {
    assert(ProtoNetwork::get_max_edges(4) == 6);
    assert(ProtoNetwork::get_max_edges(5) == 10);
    assert(ProtoNetwork::get_max_edges(6) == 15);
}

void test_mutate_node() {
    auto net = create_network();
    int counter = 0;
    const int trials = 1000;
    for (int i=0; i<trials; i++) {
        auto result = net.mutate_valid_node();
        if (result == nullptr)
            counter++;
    }
    double precision = ((double)trials - (double)counter) / trials;
    assert(precision > 0.95);
    cout << "mutate_valid_node precision: " << precision << "%\n";
}

void test_mutate_link() {
    auto net = create_network();
    int counter = 0;
    const int trials = 1000;
    for (int i=0; i<trials; i++) {
        auto result = net.mutate_valid_link();
        if (result == nullptr)
            counter++;
    }
    double precision = ((double)trials - (double)counter) / trials;
    assert(precision > 0.95);
    cout << "mutate_valid_link precision: " << precision << "%\n";
}

void test_refresh_layers() {
    auto net = create_network();
    assert(net.nodes.at(0)->layer == 0);
    assert(net.nodes.at(1)->layer == 2);
    assert(net.nodes.at(2)->layer == 0);
    assert(net.nodes.at(3)->layer == 1);
    assert(net.nodes.at(4)->layer == 1);
}

int main(int argc, char** argv) {
    try {
        test_get_max_edges();
        test_mutate_node();
        test_mutate_link();
        test_refresh_layers();
    } catch(const char* msg) { 
        std::cout << msg << std::endl; 
    }
    return 0;
}
