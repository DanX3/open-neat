#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>

using td::string;
using std::stoi;
using std::ostream;

enum Category {
    POP_SIZE = "population_size",
    THREADS = "threads_count"
};

struct Config {
    size_t threads_count;
    size_t population_size;
    size_t max_generations;
    double fitness_threshold;
    bool fitness_threshold_enabled;

    template <class T>
    std::pair<string, T> parse_line(string line) {
        size_t pos = line.find("=");
        if (pos == string::npos)
            return NULL;
        string cat = line.substr(0, pos-1);
        T val = stoi(line.substr(pos+1));
        std::pair<string, T> result = {cat, val};
        return result;
    }

    Config() {
        std::ifstream istream(filename);
        if (not istream.is_open()) {
            throw "Config file \"config\" not found\n";
        }
        do {
            string line;
            std::getline(istream, line);
            auto vals = parse_line(line);
            if (vals == NULL)
                continue;
            if (vals.first == Category::POP_SIZE)
                population_size = vals.second;
            if (cat == Category::THREADS)
                threads_count = vals.second;

        } while (not istream.eof())
    }
};

ostream& operator<<(ostream& os, const Config& c) {
    os << "Config:\n"
       << "Threads count: " << c.threads_count << endl
       << "Population size: " << c.population_size << endl;
}

 #endif
