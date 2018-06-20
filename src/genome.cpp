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

Genome::Genome(const vector<gene_ptr>& genes_) {
    genes = {};
    for (const auto& g: genes_)
        genes.insert({g->id, private_gene_t(*g, 1.0, true)});

    setup_protonet();
}

Genome::Genome(const vector<gene_t>& genes_) {
    genes = {};
    for (const auto& g: genes_)
        genes.insert({g.id, private_gene_t(g, 1.0, true)});
    setup_protonet();
}

void Genome::setup_protonet() {
    layer_0 = get_layer_0();
    proto_net = make_shared<ProtoNetwork>(layer_0);
    for (const auto& pair: genes) {
        proto_net->add_gene(pair.second.gene);
    }
}

//Genome::Genome(const Genome& src) {
    //genes = {};
    //for (auto& g: src.get_genes())
        //genes.insert({g.second.gene.id, private_gene_t(
                    //g.second.gene, g.second.weight, g.second.enabled)});
    //Genome();
//}

void Genome::set_weigth(size_t gene_id, double new_weight) {
    (genes.at(gene_id)).weight = new_weight;
}

void Genome::set_enable(size_t gene_id, bool enabled) {
    (genes.at(gene_id)).enabled = enabled;
}

set<size_t> Genome::get_layer_0() const {
    set<size_t> node_ids = {};
    cout << "Looking for layer 0" << endl;
    for (const auto& gene: genes)
        node_ids.insert(gene.second.gene.from);
    for (const auto& gene: genes) {
        auto it = node_ids.find(gene.second.gene.to);
        if (it != node_ids.end())
            node_ids.erase(it);
    }
    cout << "found the followings:" << endl;
    for (auto& i: node_ids)
        cout << i << " ";
    cout << endl;
    return node_ids;
}

ostream& operator<<(ostream& os, const Genome& g) {
    os << "Genome" << endl;
    for (const auto& gene: g.genes)
        os << gene.first << ", " << gene.second << endl;
    os << endl;
    os << *g.proto_net << endl;
    return os;
}

void Genome::add_node(gene_ptr to_new_node, gene_ptr from_new_node) {
    //proto_net(to_new_node);
}

void Genome::add_link(gene_ptr new_link_gene) {
}

gene_ptr Genome::mutate_valid_link() const {
    return proto_net->mutate_valid_link();
}

gene_ptr Genome::mutate_valid_node() const {
    return proto_net->mutate_valid_node();
}
