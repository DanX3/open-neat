#ifndef NETWORK_H
#define NETWORK_H

#include "node.h"
#include <map>
#include <algorithm>
#include <numeric>
#include <fstream>
#include <iomanip>
#include "misc.h"

using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::ostream;

class NetworkTest;

class Network {
    private:
        vector<map<size_t, Node>> net;
        bool editable;
        void reset();

    protected:
    public:
        double fitness;
        static void softmax(vector<double> &actions);
        Network(unsigned short layers_count);
        void finalize();
        void add_node(Node node, unsigned short layer);
        void link_nodes(size_t node1_id, unsigned short node1_layer, 
                        size_t node2_id, unsigned short node2_layer,
                        double weight);
        int evaluate(vector<double> input);
        vector<double> evaluate_with_actions(vector<double> input);
        vector<double> get_output();
        size_t get_layers_count() const;
        friend ostream& operator<<(ostream& os, const Network& t);
        void write_to_file(const char* filename) const;
        friend NetworkTest;
};

 #endif
