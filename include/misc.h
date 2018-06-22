#ifndef MISC_H
#define MISC_H

#include <iostream>
#include <memory>

using std::cout;
using std::ostream;
using std::endl;
using std::shared_ptr;
using std::unique_ptr;


struct gene_t;

typedef shared_ptr<gene_t> gene_ptr;

struct gene_t {
        const size_t id;
        const size_t from;
        const size_t to;
        gene_t(size_t id_, size_t from_, size_t to_) :
            id(id_), from(from_), to(to_) { }
};

inline bool operator==(const gene_t& lhs, const gene_t& rhs) {
    if (lhs.id == rhs.id 
            and lhs.from == rhs.from
            and lhs.to == rhs.to)
        return true;
    return false;
}

inline bool operator!=(const gene_t& lhs, const gene_t& rhs) {
    return not (lhs == rhs);
}

inline shared_ptr<gene_t> new_gene_t(size_t id, size_t from, size_t to) {
    return shared_ptr<gene_t>(new gene_t(id, from, to));
}


inline ostream& operator<<(ostream& os, const gene_t& g) {
    os << "Gene #" << g.id << "\n\t" << g.from << " -> " << g.to;
    return os;
}

struct private_gene_t {
    const gene_t&  gene;
    double weight;
    bool enabled;

    private_gene_t(const gene_t& gene_, double weight_, bool enabled_) :
        gene(gene_), weight(weight_), enabled(enabled_) { }
};

inline bool operator==(const private_gene_t& lhs, const private_gene_t& rhs) {
    if (lhs.gene == rhs.gene
            and lhs.weight == rhs.weight
            and lhs.enabled == rhs.enabled)
        return true;
    return false;
}

inline bool operator<(const private_gene_t& lhs, const private_gene_t&  rhs) {
    return lhs.gene.id < rhs.gene.id;
}

inline shared_ptr<private_gene_t> 
new_private_gene_t(const gene_t& g, double weight, bool enabled) {
    return shared_ptr<private_gene_t>(new private_gene_t(g, weight, enabled));
}

inline ostream& operator<<(ostream& os, const private_gene_t& g) {
    os << "Private " << g.gene 
       << "\n\tWeight : " << g.weight << " "
       << "\n\t" << (g.enabled ? "enabled" : "disabled")
       << std::endl; 
    return os;
}

struct link_t {
    size_t from;
    size_t to;
};

inline ostream& operator<<(ostream& os, const link_t& l) {
    os << "Link(" << l.from << " -> " << l.to << ")";
    return os;
}

//template<typename T, class U>
//ostream& operator<<(ostream& os, std::pair<T,U> pair) {
    //os << "<" << pair.first << ", " << pair.second << ">";
    //return os;
//}

#endif
