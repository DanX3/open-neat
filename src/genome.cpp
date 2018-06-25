#include "genome.h"

const map<size_t, private_gene_t>& Genome::get_genes() const {
    return genes;
}

Genome::Genome(map<size_t, private_gene_t> genes_) {
    genes = genes_;
    setup_protonet();
}

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
    set<size_t> nodes = {};
    set<std::pair<size_t, size_t>> links = {};
    for (const auto& pair: genes) {
        if (not pair.second.enabled)
            continue;
        auto& gene = pair.second.gene;
        nodes.insert(gene.from);
        nodes.insert(gene.to);
        links.insert({gene.from, gene.to});
    }
    proto_net = make_shared<ProtoNetwork>(layer_0, nodes, links);
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

ostream& operator<<(ostream& os, const Genome& g) {
    os << "Genome" << endl;
    for (const auto& gene: g.genes)
        os << gene.first << ", " << gene.second << endl;
    os << endl;
    os << *g.proto_net << endl;
    return os;
}

void Genome::add_node(gene_ptr to_new_node, gene_ptr from_new_node) {
    // Look for the gene with the link to be disabled
    std::unique_ptr<private_gene_t> old_gene;
    for (const auto& i: genes) {
        if (i.second.gene.from == to_new_node->from 
                and i.second.gene.to == from_new_node->to) {
            old_gene = std::make_unique<private_gene_t>(i.second);
            break;
        }
    }

    genes.insert({to_new_node->id, {*to_new_node, old_gene->weight, true}});
    genes.insert({from_new_node->id, {*from_new_node, 1.0, true}});
    old_gene->enabled = false;
}

void Genome::add_link(gene_ptr new_link_gene) {
    genes.insert({new_link_gene->id, {*new_link_gene, 1.0, true}});
    proto_net->add_mutation_link(new_link_gene);
}

gene_ptr Genome::mutate_valid_link() const {
    return proto_net->mutate_valid_link();
}

gene_ptr Genome::mutate_valid_node() const {
    return proto_net->mutate_valid_node();
}

void Genome::insert_iterator(map<size_t, private_gene_t>& genome, 
        map<size_t, private_gene_t>::iterator& it) {
    genome.insert(*it);
    advance(it, 1);
}

shared_ptr<Genome> Genome::crossover(const Genome& rhs) {
    auto& genes1 = genes;
    auto& genes2 = rhs.genes;
    map<size_t, private_gene_t> new_genome = {};
    auto g1 = genes1.begin();
    auto g2 = genes2.begin();
    while (g1 != genes1.end() or g2 != genes2.end()) {
        if (g1 == genes1.end()) {
            while (g2 != genes2.end())
                new_genome.insert(*g2++);
            break;
        }

        if (g2 == genes2.end()) {
            while (g1 != genes1.end())
                new_genome.insert(*g1++);
            break;
        }

        const private_gene_t& pg1 = (*g1).second;
        const private_gene_t& pg2 = (*g2).second;

        if (pg1.gene.id == pg2.gene.id) {
            if (pg1.enabled and pg2.enabled) {
                if (fitness < rhs.fitness) {
                    new_genome.insert(*g2++);
                    g1++;
                } else {
                    new_genome.insert(*g1++);
                    g2++;
                }
            } else if (pg1.enabled) {
                new_genome.insert(*g2++);
                g1++;
            }
            else {
                new_genome.insert(*g1++);
                g2++;
            }
        }

        if (pg1.gene.id < pg2.gene.id)
            new_genome.insert(*g1++);

        if (pg1.gene.id > pg2.gene.id)
            new_genome.insert(*g2++);
    }
    return make_shared<Genome>(new_genome);
}

void Genome::write_to_file(string filename) const {
    proto_net->write_to_file(filename.c_str());
}

network_ptr Genome::get_network() {
    if (network == nullptr)
        network = proto_net->get_network(genes);
    return network;
}
