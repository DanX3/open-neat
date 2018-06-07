#ifndef NODE_ID_GENERATOR_H
#define NODE_ID_GENERATOR_H

class NodeIDGen {
    unsigned long counter;
    public:
        static NodeIDGen& instance() {
            // Since it's a static variable, if the class has already been created,
            // it won't be created again.
            // And it **is** thread-safe in C++11.
            static NodeIDGen myInstance;

            // Return a reference to our instance.
            return myInstance;
        }

        // delete copy and move constructors and assign operators
        NodeIDGen(NodeIDGen const&) = delete;             // Copy construct
        NodeIDGen(NodeIDGen&&) = delete;                  // Move construct
        NodeIDGen& operator=(NodeIDGen const&) = delete;  // Copy assign
        NodeIDGen& operator=(NodeIDGen &&) = delete;      // Move assign

        // Any other public methods.
        unsigned long get_id() { return counter++; }
        unsigned long check_id() { return counter; }

    protected:
        NodeIDGen() {
            // Constructor code goes here.
            counter = 0;
        }

        ~NodeIDGen() {
            // Destructor code goes here.
        }

        // And any other protected methods.
};

#endif
