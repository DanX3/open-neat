#include "NEAT.h"

NEAT::NEAT(size_t input_size, size_t output_size) : 
    handler(input_size, output_size) { 
        gen_count = 0;
        log.open("log.dat");
    }

genome_ptr NEAT::train_generation() {
    gen_count++;
    size_t counter = 0;
    double avg_fitness = 0.0;
    genome_ptr genome;
    while ((genome = handler.get_genome(counter++)) != nullptr) {
        genome->fitness = play(genome->get_network());
        avg_fitness += genome->fitness;
        if (std::isnan(genome->fitness))  {
            std::cout << "Fitness is nan:" << genome->fitness 
                      << " at " << counter << '\n';
            //genome->get_network()->write_to_file("nan_network.dot");
            cout << *genome << endl;
            throw "NaN Fitness";
        }

    }
    genome_ptr best = handler.get_best_genome();
    log_gen();
    cout << "Generation #" << gen_count << endl
         << "Average : " << (avg_fitness / counter) << endl
         << "Best    : " << best->fitness << "\n\n";
    return best;
}

void NEAT::train(size_t gen_count) {
    auto settings = Config::instance().get_settings();
    if (gen_count == 0)
        gen_count = settings.max_generations;
    //for (size_t i=0; i<gen_count; i++) {
    size_t i = 0;
    do {
        auto best = train_generation();
        if (settings.fitness_threshold_enabled
                and best->fitness > settings.fitness_threshold ) {
            log_best(best);
            if (log.is_open())
                log.close();
            return;
        }

        if (i != gen_count ) {
            handler.adjust_fitness();
            handler.reproduce();
            handler.mutate_genomes();
        }
    } while (i++ < gen_count);

    log_best(handler.get_best_genome());
    if (log.is_open())
        log.close();
}

void NEAT::log_best(genome_ptr best) {
    best->write_to_file("best.dot");
    cout << "Training stopped with fitness " << best->fitness << '\n';
    showcase(best->get_network());
}


void NEAT::print_network() const {
    handler.get_genome(0)->write_to_file("network_0.dot");
}

void NEAT::log_gen() {
    genome_ptr genome;
    size_t counter = 0;
    while ((genome = handler.get_genome(counter++)) != nullptr) {
        log << gen_count << " " << genome->fitness << endl;
    }
}

