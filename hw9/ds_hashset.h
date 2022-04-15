#ifndef ds_hashset_h_
#define ds_hashset_h_

#include "Query.h"

// hash function
int HashFunc(const std::string &key, unsigned int size) {
    //  http://www.partow.net/programming/hashfunctions/

    unsigned int hash = 1315423911;
    for (unsigned int i = 0; i < key.length(); i++)
        hash ^= ((hash << 5) + key[i] + (hash >> 2));
    return hash % size;
}

class ds_hashset {
private:
    // HASH SET REPRESENTATION

    std::vector<std::pair<Query, std::list<MovieData*> > *> m_table;  // actual table
    unsigned int m_size, occupied;                        // number of keys
    float occupancy_level;

public:

    ds_hashset(unsigned int init_size) : m_table(init_size), m_size(100), occupancy_level(0.5), occupied(0) {}

    // Copy constructor just uses the member function copy constructors.
    ds_hashset(const ds_hashset &old)
            : m_table(old.m_table), m_size(old.m_size) {}

    ~ds_hashset() {
        for (unsigned int i=0; i<m_table.size(); i++) {
            if (m_table[i]){
                delete m_table[i];
            }
        }
    }

    ds_hashset &operator=(const ds_hashset &old) {
        if (&old != this)
            *this = old;
        return *this;
    }

    void change_size(int size) { m_size = size; }

    void change_occupancy_level(float f) { occupancy_level=f; }

    void insert(std::pair<Query, std::list<MovieData*>> *current_value) {
        Query current_key = current_value->first;

        if (1.0*occupied/m_size>occupancy_level) rehash();
        // Apply hash function to find index for given key
        int hashIndex = HashFunc(current_key.long_str(), m_size);

        // find next free slot if m_table[hashIndex] is occupied
        while (m_table[hashIndex]) {
            hashIndex++;
            hashIndex %= m_size;
        }

        m_table[hashIndex] = current_value;
        occupied++;
    }

    // Print output after query command
    void print(Query current_key, std::map<std::string, std::string> actorlist) {
        int hashIndex = HashFunc(current_key.long_str(), m_size);
        if (m_table[hashIndex] && m_table[hashIndex]->second.size() != 0) {
            std::cout << "Printing " << m_table[hashIndex]->second.size() << " result(s):\n";

            // print each moviedata in the list
            for (std::list<MovieData*>::iterator itr = (m_table[hashIndex]->second).begin();
                 itr != m_table[hashIndex]->second.end(); itr++) {
                (*itr)->print(actorlist); // see MovieData.cpp
            }

        }
        else {
            std::cout << "No results for query.\n"; // nothing in the list
        }

    }

    void rehash(){
        std::vector<std::pair<Query, std::list<MovieData*> > *> oldtable= m_table;
        m_table.clear();
        m_size*=2;
        for (unsigned int i=0; i<oldtable.size(); i++) {
            if (oldtable[i]){
                unsigned int index = HashFunc(oldtable[i]->first.long_str(), m_size);
                m_table[index]=oldtable[i];
            }
        }
    }

};

#endif
