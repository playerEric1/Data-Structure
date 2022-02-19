#ifndef __Order_H_
#define __Order_H_
#include <string>
#include <cassert>
#include <list>

class Order
{

public:
    // CONSTRUCTORS
    Order(int _id, int _promised_time, std::list<std::string> _order_items) : 
        id(_id), promised_time(_promised_time), order_items(_order_items) {}

    // ACCESSORS
    bool expire() const { return promised_time <= 0; } // check whether to remove
    int getTime() const { return promised_time; }
    int getId() const { return id; }
    std::list<std::string> getItems() const { return order_items; }

    // MODIFIERS
    void timepass() { promised_time--; }

private:
    // REPRESENTATION
    int id;
    int promised_time;
    std::list<std::string> order_items;
};

// sort helper
bool orders_by_time(Order &o1, Order &o2) { return o1.getTime() < o2.getTime(); }
bool orders_by_id(Order &o1, Order &o2) { return o1.getId() < o2.getId(); }

#endif