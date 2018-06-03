#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

class IDGenerator {
    private:
        size_t id;
    public:
        IDGenerator() {
            id = 0;
        }
        size_t get_id() {
            return id++;
        }

};

 #endif
