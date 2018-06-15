#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <iomanip>

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
};

class Config {
    public:
        static Config& instance() {
            static Config myInstance("config.txt");
            return myInstance;
        }
        Config(Config const&) = delete;
        Config(Config&&) = delete;
        Config& operator=(Config const&) = delete;
        Config& operator=(Config&&) = delete;
        friend ostream& operator<<(ostream& os, const Config& c);

        Configuration& get_settings() {
            return c;
        }

    private:
        typedef std::pair<string, double> setting;
        map<string, double> settings = {
            {"threads_count", 0.0}
            ,{"population_size", 0.0}
            ,{"max_generations", 0.0}
            ,{"fitness_threshold", 0.0}
            ,{"fitness_threshold_enabled", 0.0}
        };
        Configuration c;

        unique_ptr<std::pair<string, double>> parse_line(string line) {
            size_t pos = line.find("=");
            if (pos == string::npos)
                return nullptr;
            string cat = line.substr(0, pos);
            double val = stod(line.substr(pos+1));
            return unique_ptr<setting>{new setting(cat, val)};
        }

        Configuration associate_values(map<string, double> vals) {
            Configuration c = {};
            c.threads_count = (size_t)vals.at("threads_count");
            c.population_size = (size_t)vals.at("population_size");
            c.max_generations = (size_t)vals.at("max_generations");
            c.fitness_threshold = vals.at("fitness_threshold");
            c.fitness_threshold_enabled = (bool)vals.at("fitness_threshold_enabled");
            return c;
        }

        Config(string filename) {
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

};

ostream& operator<<(ostream& os, const Config& c) {
    os << "Config:\n";
    for (auto pair: c.settings) {
        os << std::setw(25) << pair.first << ": " << pair.second << endl;
    }
    cout << endl;
    return os;
}

#endif
