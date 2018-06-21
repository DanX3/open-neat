#include "proto_network.h"
#include "node_id_generator.h"
#include "id_generator.h"
#include <cassert>
#include <vector>

using std::vector;


ProtoNetwork create_network() {
    ProtoNetwork net = {{0, 2}};
    net.add_mutation_link(gene_ptr(new gene_t{IDGenerator::instance().get_id(), 0, 4}));
    net.add_mutation_link(gene_ptr(new gene_t{IDGenerator::instance().get_id(), 4, 1}));
    net.add_mutation_link(gene_ptr(new gene_t{IDGenerator::instance().get_id(), 2, 3}));
    net.add_mutation_link(gene_ptr(new gene_t{IDGenerator::instance().get_id(), 3, 1}));
    while (NodeIDGen::instance().check_id() <= 4)
        NodeIDGen::instance().get_id();
    return net;
}

void test_get_max_edges() {
    assert(ProtoNetwork::get_max_edges(4) == 6);
    assert(ProtoNetwork::get_max_edges(5) == 10);
    assert(ProtoNetwork::get_max_edges(6) == 15);
}

gene_ptr get_registered_mutated_link(const ProtoNetwork& net) {
    gene_ptr new_gene = net.mutate_valid_link();
    if (new_gene == nullptr)
        return nullptr;
    gene_ptr new_registered_gene = std::make_shared<gene_t>(
            IDGenerator::instance().get_id(), new_gene->from, new_gene->to);
    return new_registered_gene;
}

std::pair<gene_ptr, gene_ptr> get_registered_mutated_node(const ProtoNetwork& net) {
    gene_ptr new_node = net.mutate_valid_node();
    size_t new_node_id = NodeIDGen::instance().get_id();
    gene_ptr from = std::make_shared<gene_t>(
            IDGenerator::instance().get_id(), new_node->from, new_node_id);
    gene_ptr to = std::make_shared<gene_t>(
            IDGenerator::instance().get_id(), new_node_id, new_node->to);
    return {from, to};
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
    assert(precision > 0.93);
    cout << "mutate_valid_node precision: " << precision * 100.0 << "%\n";
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
    cout << "mutate_valid_link precision: " << precision * 100.0 << "%\n";
    assert(precision > 0.93);
}

void test_mutate_link_fully_connected() {
    auto net = create_network();
    net.add_mutation_link(gene_ptr(new gene_t{4, 0, 3}));
    net.add_mutation_link(gene_ptr(new gene_t{5, 2, 4}));
    net.add_mutation_link(gene_ptr(new gene_t{6, 0, 1}));
    net.add_mutation_link(gene_ptr(new gene_t{7, 2, 1}));
    for (int i=0; i<100; i++) {
        assert(net.mutate_valid_link() == nullptr);
    }
}

void test_refresh_layers() {
    auto net = create_network();
    assert(net.nodes.at(0)->layer == 0);
    assert(net.nodes.at(1)->layer == 2);
    assert(net.nodes.at(2)->layer == 0);
    assert(net.nodes.at(3)->layer == 1);
    assert(net.nodes.at(4)->layer == 1);
}

void heavy_test_mutation() {
    auto net = create_network();
    for (int i=0; i<100; i++) {
        auto pair = get_registered_mutated_node(net);
        net.add_mutation_node(pair.first, pair.second);
        
        for (int i=0; i<2; i++) {
            gene_ptr new_registered_gene = get_registered_mutated_link(net);
            if (new_registered_gene != nullptr)
                net.add_mutation_link(new_registered_gene);
        }
    }
}

void get_layers_count_test() {
    auto net = create_network();
    assert(net.get_layers_count() == 3);
}

int main(int argc, char** argv) {
    try {
        test_get_max_edges();
        test_mutate_node();
        test_mutate_link();
        test_refresh_layers();
        heavy_test_mutation();
        get_layers_count_test();
        test_mutate_link_fully_connected();
    } catch(const char* msg) { 
        std::cout << msg << std::endl; 
    }
    return 0;
}
