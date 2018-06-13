#include "genomes_manager.h"

GenomesHandler::GenomesHandler(size_t input_size, size_t output_size) {
    genes_list = {};
    for (size_t i=0; i<input_size; i++) {
        for (size_t j=0; j<output_size; j++) {
            genes_list.push_back(new_gene_t(
                        NodeIDGen::instance().get_id(), i, input_size+j));
        }
    }
}

ostream& operator<<(ostream& os, const GenomesHandler& gh) {
    for (auto i: gh.genes_list)
        cout << *i << endl;
    return os;
}
