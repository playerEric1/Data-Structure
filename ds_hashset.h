#ifndef ds_hashset_h_
#define ds_hashset_h_

#include "Query.h"

int HashFunc(const std::string &key, unsigned int size) {

    unsigned int hash = 1315423911;
    for (unsigned int i = 0; i < key.length(); i++)
        hash ^= ((hash << 5) + key[i] + (hash >> 2));
    return hash % size;
}

class ds_hashset {
private:
    // HASH SET REPRESENTATION
    std::vector<std::pair<Query, std::list<MovieData *> > *> m_table;  // actual table
    unsigned int occupied;                        // number of keys
    float occupancy_level;

public:

    ds_hashset(unsigned int init_size) : m_table(init_size), occupied(0), occupancy_level(0.5) {}

    // Copy constructor just uses the member function copy constructors.
    ds_hashset(const ds_hashset &old)
            : m_table(old.m_table) {}

    ~ds_hashset() {
        for (unsigned int i = 0; i < m_table.size(); i++) {
            if (m_table[i]) {
                delete m_table[i]; // delete pointer to the pair if not an empty slot
            }
        }
    }

    ds_hashset &operator=(const ds_hashset &old) {
        if (&old != this)
            *this = old;
        return *this;
    }

    int getsize() { return m_table.size(); }

    void change_size(int size) { rehash(size); }

    void change_occupancy_level(float f) { occupancy_level = f; }

    void insert(std::pair<Query, std::list<MovieData *>> *current_value) {
        Query current_key = current_value->first;

        if (1.0 * occupied / m_table.size() > occupancy_level) rehash();
        // Apply hash function to find index for given key
        int hashIndex = HashFunc(current_key.long_str(), m_table.size());

        // find next free slot if m_table[hashIndex] is occupied
        while (m_table[hashIndex]) {
            hashIndex++;
            hashIndex %= m_table.size();
        }

        m_table[hashIndex] = current_value;
        occupied++;
    }

    // Print output after query command
    void print(Query current_key, std::map<std::string, std::string> actorlist) {
        int hashIndex = HashFunc(current_key.long_str(), m_table.size());
        if (m_table[hashIndex] && m_table[hashIndex]->second.size() != 0) {
            std::cout << "Printing " << m_table[hashIndex]->second.size() << " result(s):\n";

            // print each moviedata in the list
            for (std::list<MovieData *>::iterator itr = (m_table[hashIndex]->second).begin();
                 itr != m_table[hashIndex]->second.end(); itr++) {
                (*itr)->print(actorlist); // see MovieData.cpp
            }

        } else {
            std::cout << "No results for query.\n"; // nothing in the list
        }

    }

    void rehash(int size = 0) {
        std::vector<std::pair<Query, std::list<MovieData *> > *> oldtable = m_table;

        if (size == 0) size = m_table.size() * 2; // if no value specified, double the capacity
        m_table.clear();
        m_table.resize(size);
        for (unsigned int i = 0; i < oldtable.size(); i++) {

            if (oldtable[i]) {
                int index = HashFunc(oldtable[i]->first.long_str(), m_table.size());

                while (m_table[index]) {
                    index++;
                    index %= m_table.size();
                }
                m_table[index] = oldtable[i]; // reassign all slots in the old table
            }

        }
    }

};

#endif