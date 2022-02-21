#ifndef __unrolled_H_
#define __unrolled_H_
#include <string>
#include <cassert>
#include <ostream>

const int NUM_ELEMENTS_PER_NODE = 6;

template <class T>
class Node
{
public:
  Node() : next_(NULL), prev_(NULL) {}

  // REPRESENTATION
  int num_elements=1; // no empty node exists
  T value_[NUM_ELEMENTS_PER_NODE];
  Node<T> *next_;
  Node<T> *prev_;
};

// A "forward declaration" of this class is needed
template <class T>
class UnrolledLL;

// -----------------------------------------------------------------
// LIST ITERATOR
template <class T>
class list_iterator
{
public:
  // default constructor, copy constructor, assignment operator, & destructor
  list_iterator(Node<T> *p = NULL) : ptr_(p) {}
  // NOTE: the implicit compiler definitions of the copy constructor,
  // assignment operator, and destructor are correct for this class

  // dereferencing operator gives access to the value at the pointer
  T &operator*() { return ptr_->value_[offset]; }

  // increment & decrement operators
  list_iterator<T> &operator++()
  { // pre-increment, e.g., ++iter
    if (offset == ptr_->num_elements - 1)
    {
      ptr_ = ptr_->next_;
      offset = 0;
    }
    else
    {
      offset++;
    }
    return *this;
  }
  list_iterator<T> operator++(int)
  { // post-increment, e.g., iter++
    list_iterator<T> temp(*this);
    if (offset == ptr_->num_elements - 1)
    {
      ptr_ = ptr_->next_;
      offset = 0;
    }
    else
    {
      offset++;
    }
    return temp;
  }
  list_iterator<T> &operator--()
  { // pre-decrement, e.g., --iter
    if (offset == 0)
    {
      ptr_ = ptr_->prev_;
      offset = ptr_->num_elements - 1;
    }
    else
    {
      offset--;
    }
    return *this;
  }
  list_iterator<T> operator--(int)
  { // post-decrement, e.g., iter--
    list_iterator<T> temp(*this);
    if (offset == 0)
    {
      ptr_ = ptr_->prev_;
      offset = ptr_->num_elements - 1;
    }
    else
    {
      offset--;
    }
    return temp;
  }
  // the UnrolledLL class needs access to the private ptr_ member variable
  friend class UnrolledLL<T>;

  // Comparions operators are straightforward
  bool operator==(const list_iterator<T> &r) const
  {
    return (ptr_ == r.ptr_) && (offset == r.offset);
  }
  bool operator!=(const list_iterator<T> &r) const
  {
    return (ptr_ != r.ptr_) || (offset != r.offset);
  }

private:
  // REPRESENTATION
  Node<T> *ptr_; // ptr to node in the list
  int offset=0;    // index to array
};

// -----------------------------------------------------------------
// LIST CLASS DECLARATION
// Note that it explicitly maintains the size of the list.
template <class T>
class UnrolledLL
{
public:
  // default constructor, copy constructor, assignment operator, & destructor
  UnrolledLL() : head_(NULL), tail_(NULL), size_(0) {}
  UnrolledLL(const UnrolledLL<T> &old) { copy_list(old); }
  UnrolledLL &operator=(const UnrolledLL<T> &old);
  ~UnrolledLL() { destroy_list(); }

  typedef list_iterator<T> iterator;

  // simple accessors & modifiers
  unsigned int size() const { return size_; }
  bool empty() const { return head_ == NULL; }
  void clear() { destroy_list(); }

  // read/write access to contents
  // crash when empty list
  const T &front() const { return head_->value_[0]; }
  T &front() { return head_->value_[0]; }
  const T &back() const { return tail_->value_[tail_->num_elements-1]; }
  T &back() { return tail_->value_[tail_->num_elements-1]; }

  // modify the linked list structure
  void push_front(const T &v);
  void pop_front();
  void push_back(const T &v);
  void pop_back();

  iterator erase(iterator itr);
  iterator insert(iterator itr, const T &v);
  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(NULL); }

  // print helper
  void print(std::ostream &os);

private:
  // private helper functions
  void copy_list(const UnrolledLL<T> &old);
  void destroy_list();

  // REPRESENTATION
  Node<T> *head_;
  Node<T> *tail_;
  unsigned int size_=0;
};

// -----------------------------------------------------------------
// LIST CLASS IMPLEMENTATION
template <class T>
UnrolledLL<T> &UnrolledLL<T>::operator=(const UnrolledLL<T> &old)
{
  // check for self-assignment
  if (&old != this)
  {
    destroy_list();
    copy_list(old);
  }
  return *this;
}

template <class T> // tested
void UnrolledLL<T>::push_front(const T &v)
{
  // special case: initially empty list
  if (!tail_)
  {
    Node<T> *tmp = new Node<T>;
    head_ = tail_ = tmp;
    tmp->value_[0]=v;
  }

  // head node is full
  else if (head_->num_elements==NUM_ELEMENTS_PER_NODE)
  {
    // test if all nodes all full
    bool all_full=true;
    for (Node<T> *i = head_; i!=tail_; i=i->next_)
    {
      if(i->num_elements!=NUM_ELEMENTS_PER_NODE)
      {all_full=false;}
    }

    // add a new node at the beginning if all nodes are full
    if (all_full){
      Node<T> *tmp = new Node<T>;
      tmp->next_ = head_;
      head_->prev_ = tmp;
      head_ = tmp;
      head_->value_[0]=v;
    }

    else{
      T middle=v;
      T right;
      // move all values backwards
      for (iterator itr = this->begin(); itr != this->end(); itr++) {
        // printf ("Pr%9d ", middle);
        // printf ("Precs: %9d ", itr.offset);
        // printf ("Preceding with blanks: %9d \n", itr.ptr_->num_elements);

        // stop if there is a empty postion
        if (itr.offset >= itr.ptr_->num_elements)
        {
          itr.ptr_->num_elements++;
          break;
        }
        right=*itr;
        *itr=middle;
        middle=right;
    }
    }
  }
  else
  {
    // normal case: head node not full
    // 2 3 4 5 [] [] -> 1 2 3 4 5 []
    for (int i=head_->num_elements;i>0;i--)
    {
      head_->value_[i]=head_->value_[i-1];
    }
    head_->value_[0]=v;
    head_->num_elements++;
  }
  ++size_;
}

template <class T> // tested
void UnrolledLL<T>::pop_front()
{
  // only value in the list
  if (size_ == 1) 
  {
    delete head_;
    head_ = tail_ = NULL;
  }

  // only value in the node
  else if (head_->num_elements==1) 
  {
    Node<T> *tmp = (head_->next_);
    tmp->prev_ = NULL;
    delete head_;
    head_ = tmp;
  }

  // normal case
  else
  {
    for (int i=0;i<head_->num_elements;i++)
    {
      head_->value_[i]=head_->value_[i+1];
    }
    head_->num_elements--;
  }
  size_--;
}

template <class T> // tested
void UnrolledLL<T>::push_back(const T &v)
{
  // special case: initially empty list
  if (!tail_)
  {
    Node<T> *tmp = new Node<T>;
    head_ = tail_ = tmp;
    tmp->value_[0]=v;
  }
  else if (tail_->num_elements==NUM_ELEMENTS_PER_NODE)
  {
    // the node is full
    Node<T> *tmp = new Node<T>;
    tmp->prev_ = tail_;
    tail_->next_ = tmp;
    tail_ = tmp;
    tmp->value_[0]=v;
  }
  else
  {
    // normal case: at least one node already
    tail_->value_[tail_->num_elements++]=v;
  }
  ++size_;
}

template <class T> // tested
void UnrolledLL<T>::pop_back()
{
  // only value in the list
  if (size_ == 1)
  {
    delete head_;
    head_ = tail_ = NULL;
  }

  // only value in the node
  else if (tail_->num_elements==1) 
  {
    Node<T> *tmp = (tail_->prev_);
    tmp->next_ = NULL;
    delete tail_;
    tail_ = tmp;
  }

  // normal case
  else
  {
    tail_->num_elements--;
  }
  size_--;
}

// do these lists look the same (length & contents)?
template <class T>
bool operator==(UnrolledLL<T> &left, UnrolledLL<T> &right)
{
  if (left.size() != right.size())
    return false;
  typename UnrolledLL<T>::iterator left_itr = left.begin();
  typename UnrolledLL<T>::iterator right_itr = right.begin();
  // walk over both lists, looking for a mismatched value
  while (left_itr != left.end())
  {
    if (*left_itr != *right_itr)
      return false;
    left_itr++;
    right_itr++;
  }
  return true;
}

template <class T>
bool operator!=(UnrolledLL<T> &left, UnrolledLL<T> &right) { return !(left == right); }
template <class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator itr)
{
  assert(size_ > 0);
  --size_;

  // if one element left in the node, the node will be deleted
  if (itr.ptr_->num_elements==1){
    iterator result(itr.ptr_->next_);
    // One node left in the list.
    if (itr.ptr_ == head_ && head_ == tail_)
    {
      head_ = tail_ = 0;
    }
    // Removing the head in a list with at least two nodes
    else if (itr.ptr_ == head_)
    {
      head_ = head_->next_;
      head_->prev_ = 0;
    }
    // Removing the tail in a list with at least two nodes
    else if (itr.ptr_ == tail_)
    {
      tail_ = tail_->prev_;
      tail_->next_ = 0;
    }
    // Normal remove
    else
    {
      itr.ptr_->prev_->next_ = itr.ptr_->next_;
      itr.ptr_->next_->prev_ = itr.ptr_->prev_;
    }
    delete itr.ptr_;
    return result;
  }

  // normal case
  else{
    for (int i=itr.offset;i<itr.ptr_->num_elements;i++)
    {
      itr.ptr_->value_[i]=itr.ptr_->value_[i+1];
    }
    itr.ptr_->num_elements--;
    return itr.ptr_;
  }
  
}

template <class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::insert(iterator itr, const T &v)
{
  ++size_;

  // full node
  if (itr.ptr_->num_elements==NUM_ELEMENTS_PER_NODE){
    Node<T> *p = new Node<T>();
    p->prev_ = itr.ptr_->prev_;
    p->next_ = itr.ptr_;
    itr.ptr_->prev_ = p;
    if (itr.ptr_ == head_)
      head_ = p;
    else
      p->prev_->next_ = p;
    return iterator(p);
  }

  // normal case
  else{
    itr.ptr_->num_elements++;
    for (int i=itr.ptr_->num_elements;i>itr.offset;i--)
    {
     itr.ptr_->value_[i]=itr.ptr_->value_[i-1];
    }
    itr.ptr_->value_[itr.offset]=v;
    return itr++;
  }


}

template <class T> // necessary?
void UnrolledLL<T>::copy_list(const UnrolledLL<T> &old)
{
  /*
  size_ = old.size_;
  // Handle the special case of an empty list.
  if (size_ == 0)
  {
    head_ = tail_ = 0;
    return;
  }
  // Create a new head node.
  head_ = new Node<T>();
  head_->next_=old.head_->next_;
  for (int i=0;i<old.head_->num_elements;i++)
  {
    head_[i]=old.head_->value_[i];
  }
  // tail_ will point to the last node created and therefore will move
  // down the new list as it is built
  tail_ = head_;
  // old_p will point to the next node to be copied in the old list
  Node<T> *old_p = old.head_->next_;
  // copy the remainder of the old list, one node at a time
  while (old_p)
  {
    tail_->next_ = new Node<T>();
    head_->next_=old_p->next_;
    for (int i=0;i<old.head_->num_elements;i++)
    {
      head_[i]=old_p->value_[i];
    }
    tail_->next_->prev_ = tail_;
    tail_ = tail_->next_;
    old_p = old_p->next_;
  }
  */
}

template <class T>
void UnrolledLL<T>::destroy_list()
{

  if (size_ != 0)
  {

    Node<T> *tmp = head_;
    Node<T> *end = head_->next_;
    if (end == NULL)
    {
      delete head_;
    }
    else
    {
      while (end != NULL)
      {
        delete tmp;
        tmp = end;
        end = end->next_;
      }
      delete tail_;
      head_ = tail_ = NULL;
      size_ = 0;
    }
  }
}

template <class T>
void UnrolledLL<T>::print(std::ostream &os)
{
  os << "UnrolledLL, size: "<<size_<<"\n";
  for (UnrolledLL<int>::iterator itr = this->begin(); itr != this->end(); itr++) {
    if (itr.offset==0){
      os << "\nnode:[" << itr.ptr_->num_elements <<"]";
    }
    os << " " << *itr;
  }
  os<<"\n\n";
}

#endif