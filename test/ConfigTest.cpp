#include <cassert>
#include <iostream>
#include "config.h"
using namespace std;

void test_config_double() {
    assert(Config::instance().get_settings().fitness_threshold == 1.11);
}

void test_config_bool() {
    assert(Config::instance().get_settings().fitness_threshold_enabled == false);
}

void test_config_size_t() {
    assert(Config::instance().get_settings().threads_count == 8);
}

int main(int argc, char** argv) {
    try {
        test_config_double();
        test_config_bool();
        test_config_size_t();
    } catch(const char* msg) { 
        std::cout << msg << std::endl; 
    }
    return 0;
}
