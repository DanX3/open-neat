#include "NEAT.h"

NEAT::NEAT(size_t input_size, size_t output_size) : 
    handler(input_size, output_size) { }

void NEAT::train_generation() {
    handler.mutate_genomes();
    size_t counter = 0;
    double avg_fitness = 0.0;
    genome_ptr genome;
    while ((genome = handler.get_genome(counter++)) != nullptr) {
        genome->fitness = play(genome->get_network());
        avg_fitness += genome->fitness;
    }
    cout << "Average fitness: " << (avg_fitness / counter);
    handler.reproduce();
}

void NEAT::train(size_t gen_count) {
    if (gen_count == 0)
        gen_count = Config::instance().get_settings().max_generations;
    for (size_t i=0; i<gen_count; i++) {
        train_generation();
    }
}

void NEAT::print_network() const {
    handler.get_genome(0)->write_to_file("network_0.dot");
}
