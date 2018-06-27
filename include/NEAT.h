#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <memory>
#include <thread>
#include "config.h"
//#include "network.h"
#include "genomes_manager.h"
#include <unistd.h>

using std::shared_ptr;
using std::make_shared;
using std::thread;

struct Observation;
struct Informations;

struct StepResult {
    Observation& obs;
    double reward;
    bool done;
    Informations& info;
};

class NEAT {
    private:
    protected:
        shared_ptr<Observation> obs;
        virtual double play(network_ptr net) = 0;
        GenomesHandler handler;
        void train_generation();
    public:
        NEAT(size_t input_size, size_t output_size);
        void train(size_t gen_count = 0);
};

 #endif
