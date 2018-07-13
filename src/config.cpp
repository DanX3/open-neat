#include "config.h"

Config::Config(string filename) {
    settings = {
        {"threads_count", 0.0}
        ,{"population_size", 0.0}
        ,{"max_generations", 0.0}
        ,{"fitness_threshold", 0.0}
        ,{"fitness_threshold_enabled", 0.0}
        ,{"link_mutation_chance", 0.0}
        ,{"node_mutation_chance", 0.0}
        ,{"weight_max", 0.0}
        ,{"selectiveness", 0.0}
        ,{"delta_t", 1.0}
    };
    std::ifstream istream(filename, istream.in);
    if (not istream.is_open()) {
        throw "Config file \"config\" not found\n";
    }
    do {
        string line;
        std::getline(istream, line);
        if (line == "")
            break;
        auto vals = parse_line(line);
        if (vals == nullptr)
            continue;
        if (settings.find(vals->first) == settings.end()) {
            continue;
        }
        settings.at(vals->first) = vals->second;
    } while (not istream.eof());
    c = associate_values(settings);
}

Config& Config::instance() {
    static Config myInstance("config.txt");
    return myInstance;
}

Configuration& Config::get_settings() {
    return c;
}

unique_ptr<std::pair<string, double>> Config::parse_line(string line) {
    size_t pos = line.find("=");
    if (pos == string::npos)
        return nullptr;
    string cat = line.substr(0, pos);
    double val = stod(line.substr(pos+1));
    return unique_ptr<setting>{new setting(cat, val)};
}

Configuration Config::associate_values(map<string, double> vals) {
    Configuration c = {};
    c.threads_count = (size_t)vals.at("threads_count");
    c.population_size = (size_t)vals.at("population_size");
    c.max_generations = (size_t)vals.at("max_generations");
    c.fitness_threshold = vals.at("fitness_threshold");
    c.fitness_threshold_enabled = (bool)vals.at("fitness_threshold_enabled");
    c.link_mutation_chance = (double)vals.at("link_mutation_chance");
    c.node_mutation_chance = (double)vals.at("node_mutation_chance");
    c.weight_max = (double)vals.at("weight_max");
    c.selectiveness = (double)vals.at("selectiveness");
    c.delta_t = (double)vals.at("delta_t");
    return c;
}

ostream& operator<<(ostream& os, const Config& c) {
    os << "Config:\n";
    for (auto pair: c.settings) {
        os << pair.first << ": " << pair.second << endl;
    }
    cout << endl;
    return os;
}
