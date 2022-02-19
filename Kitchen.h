#ifndef __Kitchen_H_
#define __Kitchen_H_
#include <string>
#include <cassert>

class Kitchen
{

public:
    // CONSTRUCTORS
    Kitchen(std::string _name, int _cook_time) : 
        name(_name), cook_time(_cook_time) {}

    // ACCESSORS
    bool complete() const { return cook_time <= 0; } // check whether to remove
    const std::string &getName() const { return name; }
    int getTime() const { return cook_time; }
    
    // MODIFIERS
    void timepass() { cook_time--; }

private:
    // REPRESENTATION
    std::string name;
    int cook_time;
};

// sort helper
bool kitchen_by_time(Kitchen &o1, Kitchen &o2) { return o1.getTime() < o2.getTime(); }
bool kitchen_by_name(Kitchen &o1, Kitchen &o2) { return o1.getName() < o2.getName(); }

#endif