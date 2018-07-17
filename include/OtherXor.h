#ifndef OTHER_XOR_H
#define OTHER_XOR_H


#include "NEAT.h"

double operator-(vector<double>& rhs, vector<double>& lhs);
class OtherXor : NEAT {
    protected:
        double play(network_ptr net);
        void showcase(network_ptr);
    public:
        OtherXor();
};


 #endif
