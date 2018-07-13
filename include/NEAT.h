#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <memory>
#include <thread>
#include "config.h"
//#include "network.h"
#include "genomes_manager.h"
#include <unistd.h>
#include <cmath>
#include <fstream>

using std::shared_ptr;
using std::make_shared;
using std::thread;
using std::ofstream;

class NEAT {
    private:
        void log_gen();
    protected:
        //shared_ptr<Observation> obs;
        size_t gen_count;
        ofstream log;
        GenomesHandler handler;
        virtual double play(network_ptr net) = 0;
        void train_generation();
    public:
        NEAT(size_t input_size, size_t output_size);
        void train(size_t gen_count = 0);
        void print_network() const;
};

 #endif
