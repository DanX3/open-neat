#ifndef GENOMES_HANDLER_H
#define GENOMES_HANDLER_H

#include "id_generator.h"
#include "node_id_generator.h"
#include "genome.h"

class GenomesHandler {
    private:
        //vector<Genome> genomes;
        std::vector<shared_ptr<gene_t>> genes_list;
    protected:
    public:
        GenomesHandler(size_t input_size, size_t output_size);
        friend ostream& operator<<(ostream& os, const GenomesHandler& gh);
};

 #endif
