#include "NEAT.h"

NEAT::NEAT(size_t input_size, size_t output_size) : 
    handler(input_size, output_size) { }

void NEAT::train_generation() {
    size_t counter = 0;
    network_ptr net;
    while ((net = handler.get_network(counter++)) != nullptr) {
        net->fitness = play(net);
        cout << "Network #" << counter << " Fitness " << net->fitness << endl;
    }
}

void NEAT::train(size_t gen_count) {
    if (gen_count == 0)
        gen_count = Config::instance().get_settings().max_generations;
    for (size_t i=0; i<gen_count; i++) {
        train_generation();
    }
}
