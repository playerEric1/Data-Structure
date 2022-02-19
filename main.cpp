#include <cassert>
#include <iostream>
#include <string>
#include <list>
#include "Order.h"
#include "Kitchen.h"

typedef std::list<Order> OrderList;
typedef std::list<Kitchen> KitchenList;

// Needed for CanFillOrder()
typedef std::list<KitchenList::const_iterator> OrderFillList;

// Helper function
// Returns true if order can be fulfilled, and false otherwise. If true, then
// items_to_remove has iterators to kitchen_completed for all items that are used
// in the order.
bool CanFillOrder(const Order &order, const KitchenList &kitchen_completed,
                  OrderFillList &items_to_remove);
int run_until_next(KitchenList &food_cooking, KitchenList &food_completed, OrderList &orders, bool loop, bool checkfor0);
void print_orders(OrderList &orders);
void print_kitchen(KitchenList &kitchens, bool complete);


int main()
{
  OrderList orders;
  KitchenList food_cooking;
  KitchenList food_completed;

  std::string token;
  while (std::cin >> token)
  {

    if (token == "add_order")
    {
      int id, promised_time, n_items = 0;
      std::string next_item;
      std::list<std::string> order_items;

      std::cin >> id >> promised_time >> n_items;
      assert(n_items > 0);

      // read in item strings for n_items times
      for (int i = 0; i < n_items; i++)
      {
        std::cin >> next_item;
        order_items.push_back(next_item);
      }

      // add order to order list
      orders.push_back(Order(id, promised_time, order_items));
      std::cout << "Received new order #" << id << " due in " << promised_time << " minute(s):\n";

      // print each item in that order
      std::list<std::string>::const_iterator itemit;
      for (itemit = order_items.cbegin(); itemit != order_items.cend(); itemit++)
      {
        std::cout << "  " << *itemit << "\n";
      }

      //This list should be kept sorted by shortest time remaining and then ID
      orders.sort(orders_by_id);
      orders.sort(orders_by_time);
    }

    else if (token == "add_item")
    {

      int cook_time = -1;
      std::string name;
      std::cin >> cook_time >> name;
      assert(cook_time >= 0);

      food_cooking.push_back(Kitchen(name, cook_time));
      std::cout << "Cooking new " << name << " with " << cook_time << " minute(s) left.\n";

      //This list is sorted by smallest remaining cook time, then by names alphabetically
      food_cooking.sort(kitchen_by_name);
      food_cooking.sort(kitchen_by_time);
    }

    else if (token == "print_orders_by_time")
    {
      // outputs the sorted order list
      std::cout << "Printing " << orders.size() << " order(s) by promised time remaining:\n";
      print_orders(orders);
    }

    else if (token == "print_orders_by_id")
    {
      // outputs the order list sorted by smallest ID first
      std::cout << "Printing " << orders.size() << " order(s) by ID:\n";
      orders.sort(orders_by_id);
      print_orders(orders);
    }

    else if (token == "print_kitchen_is_cooking")
    {
      /* outputs the list of items that have not finished cooking
      (can include items with 0 minutes left) */
      std::cout << "Printing " << food_cooking.size() << " items being cooked:\n";
      print_kitchen(food_cooking, false);
    }

    else if (token == "print_kitchen_has_completed")
    {
      /* outputs the items that have been completely cooked in the order they were completed*/
      std::cout << "Printing " << food_completed.size() << " completely cooked items:\n";
      print_kitchen(food_completed, true);
    }

    else if (token == "run_for_time")
    {
      /* advances the time by run until time minutes, resolving all events
      also updates the times remaining on orders and items that are cooking. */
      int run_time = 0;
      std::cin >> run_time;
      assert(run_time >= 0);
      std::cout << "===Starting run of " << run_time << " minute(s)===\n";

      // only check events, do not update time
      if (run_time==0){
        run_until_next(food_cooking, food_completed, orders, true, true);
      }

      // loop run_until_next for run_time times
      for (int i = run_time; i > 0; i--)
      {

        if (run_until_next(food_cooking, food_completed, orders, true, false) == -1)
        {
          break; // exit loop if no more events will happen
        }

      }
      std::cout << "===Run for specified time is complete===\n";
    }

    else if (token == "run_until_next")
    {
      // will advance time until the next event occurs, and resolves the event
      std::cout << "Running until next event.\n";
      int keeprunning = 0;
      int count = 0;

      // check for 0 minutes event, do not update time
      if (run_until_next(food_cooking, food_completed, orders, false, true) != 1)
      // check other events
      {
        while (keeprunning == 0)
        {
          count++;
          keeprunning = run_until_next(food_cooking, food_completed, orders, false, false);
        }
      }

      // print result based on the return value of run_until_next
      if (keeprunning != -1)
      {
        std::cout << count << " minute(s) have passed.\n";
      }
      else
      {
        std::cout << "No events waiting to process.\n";
      }
    }
  }

  return 0;
}

bool CanFillOrder(const Order &order, const KitchenList &kitchen_completed,
                  OrderFillList &items_to_remove)
{
  items_to_remove.clear(); // We will use this to return iterators in kitchen_completed

  // Simple solution is nested for loop, but I can do better with sorting...

  std::list<std::string> order_items = order.getItems();
  order_items.sort();

  std::list<std::string>::const_iterator item_it;
  std::string prev_item = "";
  KitchenList::const_iterator kitchen_it;

  for (item_it = order_items.begin(); item_it != order_items.end(); item_it++)
  {
    bool found = false;

    /*Start back at beginnging of list if we're looking for something else
     *Thanks to sorting the order_items list copy, we know we're done with
       whatever kind of item prev_item was!*/
    if (prev_item != *item_it)
    {
      kitchen_it = kitchen_completed.begin();
      prev_item = *item_it;
    }

    /*Resume search wherever we left off last time (or beginning if it's a
    new kind of item*/
    for (; !found && kitchen_it != kitchen_completed.end(); kitchen_it++)
    {
      if (kitchen_it->getName() == *item_it)
      {
        items_to_remove.push_back(kitchen_it);
        found = true;
      }
    }

    // If we failed to satisfy an element of the order, no point continuing the search
    if (!found)
    {
      break;
    }
  }

  // If we couldn't fulfill the order, return an empty list
  if (items_to_remove.size() != order_items.size())
  {
    items_to_remove.clear();
    return false;
  }

  return true;
}

void print_orders(OrderList &orders)
{
  //go through the list of orders
  OrderList::const_iterator it;
  for (it = orders.cbegin(); it != orders.cend(); it++)
  {

    std::cout << "  #" << it->getId() << " (" << it->getTime() << " minute(s) left):\n";

    //print each item in that order
    std::list<std::string> item = it->getItems();
    std::list<std::string>::const_iterator itemit;
  
    for (itemit = item.cbegin(); itemit != item.cend(); itemit++)
    {
      std::cout << "    " << *itemit << "\n";
    }
  }
}

// complete==true when print_kitchen_has_completed
void print_kitchen(KitchenList &kitchens, bool complete)
{
  kitchens.sort(kitchen_by_time);

  //go through each item in the kitchen list
  KitchenList::const_iterator it;
  for (it = kitchens.cbegin(); it != kitchens.cend(); it++)
  {
    std::cout << "  " << it->getName();

    // print time remaining if we are printing item being cooked
    if (!complete)
    {
      std::cout << " (" << it->getTime() << " minute(s) left)";
    }
    std::cout << "\n";
  }
}


// the function process events and update time (1min or 0min, based on checkfor0)

// return -1 if there is no event waiting to process
// return 0 if there are events wating to process (default)
// return 1 if an event happened and the function should not be called again

// checkfor0==1: do not update time, only process events

// loop==true: run_for_time, keep returning 0 and continue looping
// loop==false: run_until_next, if one event take place, the function will return 1 and make the loop stop
int run_until_next(KitchenList &food_cooking, KitchenList &food_completed, OrderList &orders, bool loop, bool checkfor0)
{

  int ret = 0; // if ret == 1, do not process any event, but update time, used when token=="run_until_next"

  food_cooking.sort(kitchen_by_time);
  food_cooking.sort(kitchen_by_name);

  // stop the loop immediately if no events will happen
  if (food_cooking.empty() && orders.empty())
  {
    return -1;
  }

  /*  1. If a kitchen item that is being cooked can be completed,
  it should be removed from the list of items being cooked and
  added to the end of the list of items that are completely cooked.*/
  for (std::list<Kitchen>::iterator it = food_cooking.begin(); it != food_cooking.end(); it++)
  {

    if (!checkfor0)
    {
      (*it).timepass(); // update time 
    }

    if ((*it).complete() && ret!=1)
    {
      std::cout << "Finished cooking " << (*it).getName() << "\n";

      food_completed.push_back(*it); // move it to completed list
      it = food_cooking.erase(it); // remove it from cooking list
      // at the same time reset the iterator

      if (!loop)
      {
        ret = 1;
      }
      it--;
    }
  }

  /*2. If an order can be fulfilled (starting with lowest promised
  time remaining), that order is resolved. For this to happen,
  all items (be careful –there might be duplicates) required by
  the order must be in the list of completely cooked items.
  If an order is resolved, it is removed from the orders list,
  and all items that were used (prefer oldest first if there’s duplicates)
  are removed from the completely cooked item list.*/
  orders.sort(orders_by_id);
  orders.sort(orders_by_time);
  OrderFillList items_to_remove;
  for (std::list<Order>::iterator it = orders.begin(); it != orders.end(); it++)
  {

    if (!checkfor0)
    {
      (*it).timepass();
    }

    if (CanFillOrder((*it), food_completed, items_to_remove) && ret!=1)
    {
      std::cout << "Filled order #" << (*it).getId() <<"\n";
      for (OrderFillList::iterator removeit = items_to_remove.begin(); removeit != items_to_remove.end(); removeit++)
      {
        std::string printremove = (**removeit).getName();
        food_completed.erase(*removeit);
        std::cout << "Removed a "
                  << printremove
                  << " from completed items.\n";
      }
      it = orders.erase(it); // remove and reset the iterator

      if (!loop)
      {
        ret = 1;
      }
    }

    /*3. If an order cannot be fulfilled and has 0 minutes remaining,
    remove it from the orders list.*/
    else if ((*it).expire() && ret!=1)
    {
      std::cout << "Order # " << (*it).getId() << " expired.\n";
      it = orders.erase(it); // remove and reset the iterator

      if (!loop)
      {
        ret = 1;
      }
    }
  }
  return ret;
}