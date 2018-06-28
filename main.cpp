#include "OtherXor.h"

int main(int argc, char *argv[]) {
    size_t gen_count = 0;
    if (argc == 2)  {
        gen_count = std::stoi(argv[1]);
    }
    OtherXor XOR;
    XOR.train(gen_count);
    XOR.print_network();
    return 0;
}

