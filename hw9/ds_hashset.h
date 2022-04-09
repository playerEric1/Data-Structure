#ifndef ds_hashset_h_
#define ds_hashset_h_
// The set class as a hash table instead of a binary search tree.  The
// primary external difference between ds_set and ds_hashset is that
// the iterators do not step through the hashset in any meaningful
// order.  It is just the order imposed by the hash function.
#include "Query.h"

typedef typename std::vector<std::pair<Query, std::list<MovieData> > >::iterator hash_list_itr;

// hash function
int HashFunc(const std::string &key) {
    //  http://www.partow.net/programming/hashfunctions/

    unsigned int hash = 1315423911;
    for (unsigned int i = 0; i < key.length(); i++)
        hash ^= ((hash << 5) + key[i] + (hash >> 2));
    return hash;
}

class ds_hashset {
private:
    // HASH SET REPRESENTATION
    // std::vector< std::list<KeyType> >
    std::vector<std::pair<Query, std::list<MovieData> > *> m_table;  // actual table
    unsigned int m_size, capacity;                        // number of keys

public:

    // The iterator class is defined as a nested class and does not have
    // to be templated separately over the hash function object type.
    class iterator {
    public:
        friend class ds_hashset;   // allows access to private variables
    private:
        // ITERATOR REPRESENTATION
        ds_hashset *m_hs;
        int m_index;               // current index in the hash table
        hash_list_itr m_list_itr;  // current iterator at the current index

    private:
        // private constructors for use by the ds_hashset only
        iterator(ds_hashset *hs) : m_hs(hs), m_index(-1) {}

        iterator(ds_hashset *hs, int index, hash_list_itr loc)
                : m_hs(hs), m_index(index), m_list_itr(loc) {}

    public:
        // Ordinary constructors & assignment operator
        iterator() : m_hs(0), m_index(-1) {}

        iterator(iterator const &itr)
                : m_hs(itr.m_hs), m_index(itr.m_index), m_list_itr(itr.m_list_itr) {}

        iterator &operator=(const iterator &old) {
            m_hs = old.m_hs;
            m_index = old.m_index;
            m_list_itr = old.m_list_itr;
            return *this;
        }

        // The dereference operator need only worry about the current
        // list iterator, and does not need to check the current index.
        const std::pair<Query, std::list<MovieData> > &operator*() const { return *m_list_itr; }

        // The comparison operators must account for the list iterators
        // being unassigned at the end.
        friend bool operator==(const iterator &lft, const iterator &rgt) {
            return lft.m_hs == rgt.m_hs && lft.m_index == rgt.m_index &&
                   (lft.m_index == -1 || lft.m_list_itr == rgt.m_list_itr);
        }

        friend bool operator!=(const iterator &lft, const iterator &rgt) {
            return lft.m_hs != rgt.m_hs || lft.m_index != rgt.m_index ||
                   (lft.m_index != -1 && lft.m_list_itr != rgt.m_list_itr);
        }

    };


    // =================================================================
    // HASH SET IMPLEMENTATION

    // Constructor for the table set just accepts the size of the table.
    // The default constructor for the hash function object is
    // implicitly used.
    ds_hashset(unsigned int init_size) : m_table(init_size), m_size(0) {}

    // Copy constructor just uses the member function copy constructors.
    ds_hashset(const ds_hashset &old)
            : m_table(old.m_table), m_size(old.m_size) {}

    ~ds_hashset() {}

    ds_hashset &operator=(const ds_hashset &old) {
        if (&old != this)
            *this = old;
    }

    unsigned int size() const { return m_size; }

    void change_size(int size) { m_size = size; }

    void insert(std::pair<Query, std::list<MovieData>> current_value) {
        Query current_key = current_value.first;
        // Apply hash function to find index for given key
        int hashIndex = 1;
//        int hashIndex = HashFunc(current_key.long_str());

        // find next free slot if m_table[hashIndex] is occupied
        while (m_table[hashIndex] != NULL
               && m_table[hashIndex]->first.long_str() != current_key.long_str()
               && m_table[hashIndex]->first.long_str() != "") {
            hashIndex++;
            hashIndex %= capacity;
        }

        // if new node to be inserted
        // increase the current size
//        if (!m_table[hashIndex]
//            || m_table[hashIndex].first.long_str() == "")
//            size++;
        m_table[hashIndex] = &current_value;
//        m_size++;
    }

    // Create an end iterator.
    iterator end() {
        iterator p(this);
        p.m_index = -1;
        return p;
    }

    void print(Query current_key, std::map<std::string, std::string> actorlist) {
        int hashIndex = 1;
//        int hashIndex = HashFunc(current_key.long_str());
        if (m_table[hashIndex]->second.size() != 0) {
            std::cout << "Printing " << m_table[hashIndex]->second.size() << " result(s):\n";

//            for (std::list<MovieData>::iterator itr = m_table[hashIndex]->second.begin();
//                 itr != m_table[hashIndex]->second.end(); itr++) {
//                std::cout << itr->gettitle() << " result(s):\n";
//                itr->print(actorlist); // see MovieData.cpp
//            }

        } else {
            std::cout << "No results for query.\n"; // nothing in the value
        }
    }

};

#endif
