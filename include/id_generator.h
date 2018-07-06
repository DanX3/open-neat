#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

typedef unsigned long size_t;

class IDGenerator {
    unsigned long counter;
    public:
        static IDGenerator& instance() {
            // Since it's a static variable, if the class has already been created,
            // it won't be created again.
            // And it **is** thread-safe in C++11.
            static IDGenerator myInstance;

            // Return a reference to our instance.
            return myInstance;
        }

        // delete copy and move constructors and assign operators
        IDGenerator(IDGenerator const&) = delete;             // Copy construct
        IDGenerator(IDGenerator&&) = delete;                  // Move construct
        IDGenerator& operator=(IDGenerator const&) = delete;  // Copy assign
        IDGenerator& operator=(IDGenerator &&) = delete;      // Move assign

        // Any other public methods.
        size_t get_id() { return counter++; }
        size_t check_id() { return counter; }

    protected:
        IDGenerator() {
            // Constructor code goes here.
            counter = 0;
        }

        ~IDGenerator() {
            // Destructor code goes here.
        }

        // And any other protected methods.
};

#endif
