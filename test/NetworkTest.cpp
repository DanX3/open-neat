#include "network.h"
#include <cassert>

class NetworkTest {
public:

    Network create_network() {
        Network n = {3};
        n.add_node({0}, 0);
        n.add_node({1}, 0);
        n.add_node({2}, 1);
        n.add_node({4}, 1);
        n.add_node({3}, 2);
        n.link_nodes(0, 0, 2, 1, 1.0);
        n.link_nodes(0, 0, 4, 1, 1.0);
        n.link_nodes(1, 0, 2, 1, 1.0);
        n.link_nodes(1, 0, 4, 1, 1.0);
        n.link_nodes(4, 1, 3, 2, 1.0);
        n.finalize();
        return n;
    }

    void softmax_test() {
        vector<double> v = {4.1, 3.9, 3.95, 4.05};
        Network::softmax(v);
        double sum = std::accumulate(v.begin(), v.end(), 0.0);
        assert(std::abs(sum - 1.0) < 1e-6);
    }

    void evaluate_test() {
        Network n = {3};
        n.add_node({0}, 0);
        n.add_node({1}, 0);
        n.add_node({2}, 0);
        n.add_node({3}, 1);
        n.add_node({4}, 1);
        n.add_node({5}, 1);
        n.add_node({6}, 2);
        n.add_node({7}, 2);
        n.link_nodes(0, 0, 3, 1, 1.0);
        n.link_nodes(0, 0, 4, 1, 0.5);
        n.link_nodes(1, 0, 5, 1, 1.0);
        n.link_nodes(2, 0, 4, 1, 0.5);
        n.link_nodes(3, 1, 6, 2, 0.5);
        n.link_nodes(4, 1, 6, 2, 0.5);
        n.link_nodes(5, 1, 7, 2, 2.0);
        n.finalize();
        assert(n.evaluate({1.0, 1.0, 1.0}) == 1);
    }

    void get_output_nodes_test() {
        auto n = create_network();
        auto output = n.get_output();
        assert(output.size() == 2);
        assert(output.at(0) == 0);
        assert(output.at(1) == 0);

    }

    void reset_test() {
        Network n = create_network();
        auto result = n.evaluate_with_actions({1.0, 0.0});
        n.reset();
        result = n.get_output();
        assert(result[0] == 0.0);
        assert(result[1] == 0.0);
    }

};

int main(int argc, char** argv) {
    NetworkTest test;
    try {
        test.softmax_test();
        test.evaluate_test();
        test.get_output_nodes_test();
        test.reset_test();
    } catch(const char* msg) { 
        std::cout << msg << std::endl; 
    }
    return 0;
}
