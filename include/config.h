#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>

using std::string;
using std::map;
using std::stod;
using std::ostream;
using std::unique_ptr;
using std::cout;
using std::endl;

struct Configuration {
    size_t threads_count;
    size_t population_size;
    size_t max_generations;
    double fitness_threshold;
    bool fitness_threshold_enabled;
    double link_mutation_chance;
    double node_mutation_chance;
};

class Config {
    public:
        static Config& instance();
        Config(Config const&) = delete;
        Config(Config&&) = delete;
        Config& operator=(Config const&) = delete;
        Config& operator=(Config&&) = delete;
        friend ostream& operator<<(ostream& os, const Config& c);
        Configuration& get_settings();

    private:
        typedef std::pair<string, double> setting;
        map<string, double> settings;
        Configuration c;
        unique_ptr<std::pair<string, double>> parse_line(string line);
        Configuration associate_values(map<string, double> vals);
        Config(string filename);
        friend ostream& operator<<(ostream& os, const Config& c);

};


#endif
