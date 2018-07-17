#include "genome.h"

const map<size_t, private_gene_t>& Genome::get_genes() const {
    return genes;
}

Genome::Genome(map<size_t, private_gene_t> genes_) {
    genes = genes_;
    setup_protonet();
}

Genome::Genome(const set<gene_ptr>& genes_) {
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
    fitness = 0.0;
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
    mutate_weights();
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
    private_gene_t* old_gene;
    for (auto& i: genes) {
        if (i.second.gene.from == to_new_node->from 
                and i.second.gene.to == from_new_node->to) {
            old_gene = &i.second;
            break;
        }
    }

    genes.insert({to_new_node->id, {*to_new_node, old_gene->weight, true}});
    genes.insert({from_new_node->id, {*from_new_node, 1.0, true}});
    old_gene->enabled = false;
    proto_net->add_mutation_node(to_new_node, from_new_node);
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
            continue;
        }

        if (pg1.gene.id < pg2.gene.id)
            new_genome.insert(*g1++);
        else
            new_genome.insert(*g2++);
    }
    return make_shared<Genome>(new_genome);
}

void Genome::write_to_file(string filename) const {
    //proto_net->write_to_file(filename.c_str());
    proto_net->get_network(genes)->write_to_file(filename);
}

network_ptr Genome::get_network() const {
    //if (network == nullptr)
        //network = proto_net->get_network(genes);
    return proto_net->get_network(genes);
}

void Genome::mutate_weights() {
    double w_max = Config::instance().get_settings().weight_max;
    for (auto& gene: genes)
        if (randf() < 0.8) {
            if (randf() < 0.9)
                gene.second.weight = clamp(gene.second.weight 
                        + (2.0*randf() - 1.0), -w_max, w_max);
            else {
                gene.second.weight = (2.0*randf()-1.0) * w_max;
            }
        }
}

double Genome::delta(const Genome& rhs) const {
    double excess = 0.0;
    double disjoint = 0.0;
    double accumulator = 0.0;
    double accumulated = 0.0;

    auto g1 = genes.begin();
    auto g2 = rhs.genes.begin();
    while (g1 != genes.end() and g2 != rhs.genes.end()) {
        const private_gene_t& pg1 = (*g1).second;
        const private_gene_t& pg2 = (*g2).second;

        if (pg1.gene.id == pg2.gene.id) {
            accumulator += fabs(pg1.weight - pg2.weight);
            accumulated += 1;
            g1++;
            g2++;
        } else {
            if (pg1.gene.id < pg2.gene.id)
                g1++;
            else
                g2++;
            disjoint++;
        }
    }

    while (g1 != genes.end()) {
        g1++;
        excess++;
    }
    while (g2 != rhs.genes.end()) {
        g2++;
        excess++;
    }
    size_t N = max(genes.size(), rhs.genes.size());
    double W = accumulator / accumulated;
    return (c1*excess + c2*disjoint) / N + c3*W;
}

void Genome::reset_weights() {
    for (auto& i: genes) {
        i.second.weight = 1.0;
    }
}

bool Genome::is_compatible(const Genome& rhs) {
    if (delta(rhs) < delta_t)
        return true;
    return false;
}

bool Genome::operator<(const Genome& rhs) {
    return (fitness < rhs.fitness) ? true : false;
}

size_t Genome::get_size() const {
    return genes.size();
}
