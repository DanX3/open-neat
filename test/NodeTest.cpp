#include "node.h"
#include <cassert>

void node_link_test() {
    Node n1 = {1};
    Node n2 = {2};
    Node n3 = {3};
    n1.link_to(&n2, 1.0);
    n1.link_to(&n3, -1.0);
    n1.receive_input(100.0);
    n1.send_input();
    assert(n2.get_accumulator() == 100.0);
    assert(n3.get_accumulator() == -100.0);

    n2.reset();
    assert(n2.get_accumulator() == 0.0);
}

int main(int argc, char** argv) {
    try {
        node_link_test();
    } catch(const char* msg) { 
        std::cout << msg << std::endl; 
        return 1;
    }
    return 0;
}
