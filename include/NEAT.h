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
using std::ostream;

class NEAT {
    private:
        void log_gen();
        void log_best(genome_ptr best);
    protected:
        //shared_ptr<Observation> obs;
        size_t gen_count;
        ofstream log;
        GenomesHandler handler;
        virtual double play(network_ptr net) = 0;
        virtual void showcase(network_ptr net) = 0;
        genome_ptr train_generation();
    public:
        NEAT(size_t input_size, size_t output_size);
        void train(size_t gen_count = 0);
        void print_network() const;
        friend ostream& operator<<(ostream& os, const NEAT& t);
};

 #endif
