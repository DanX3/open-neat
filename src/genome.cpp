#include "genome.h"

const map<size_t, private_gene_t>& Genome::get_genes() const {
    return genes;
}

//Genome& Genome::crossover(const Genome& rhs) const {
//}

//Genome::Genome(const vector<gene_ptr>& genes_) {
    //genes = {};
    //for (const auto& g: genes_)
        //add_gene(*g, 1.0, true);
//}

Genome::Genome() {
    layer_0 = get_layer_0();
    proto_net = make_unique<ProtoNetwork>(layer_0);
}

Genome::Genome(const vector<gene_t>& genes_) {
    genes = {};
    for (const auto& g: genes_)
        add_gene(g, 1.0, true);
    for (const auto& it: get_layer_0())
        cout << "In layer 0: " << it << endl;
    Genome();
}

Genome::Genome(const Genome& src) {
    genes = {};
    for (auto& g: src.get_genes())
        add_gene(g.second.gene, g.second.weight, g.second.enabled);
    Genome();
}

void Genome::add_gene(const gene_t& g, double weight, bool enabled) {
    genes.insert({g.id, private_gene_t(g, weight, enabled)});
}

void Genome::to_string() const {
    for (const auto& gene: genes) {
        cout << gene.second << endl;
    }
}

void Genome::set_weigth(size_t gene_id, double new_weight) {
    (genes.at(gene_id)).weight = new_weight;
}

void Genome::set_enable(size_t gene_id, bool enabled) {
    (genes.at(gene_id)).enabled = enabled;
}

set<size_t> Genome::get_layer_0() const {
    set<size_t> node_ids = {};
    for (const auto& gene: genes)
        node_ids.insert(gene.second.gene.from);
    for (const auto& gene: genes) {
        auto it = node_ids.find(gene.second.gene.to);
        if (it != node_ids.end())
            node_ids.erase(it);
    }
    return node_ids;
}
gene_ptr Genome::mutation_link_proposal() const {
    return nullptr;;
}
