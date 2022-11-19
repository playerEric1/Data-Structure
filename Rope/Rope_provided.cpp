#include <cassert>
#include "Rope.h"

bool is_leaf(Node* p){
	return p && !(p->left) && !(p->right);
}

///////////// rope_iterator functions /////////////

//Relies on rope_iterator::operator++()
rope_iterator rope_iterator::operator++(int){
    rope_iterator temp(*this);
    ++(*this);
    return temp;
}

rope_iterator::rope_iterator(){
	ptr_ = NULL;
}

rope_iterator::rope_iterator(Node* p){
	ptr_ = p;
}

int rope_iterator::weight() const{
	return ptr_->weight;
}

const std::string& rope_iterator::value() const{
	return ptr_->value;
}

bool rope_iterator::operator!=(const rope_iterator& it2) const{
	return ptr_ != it2.ptr_;
}

///////////// Rope functions /////////////

rope_iterator Rope::end() const{
	return rope_iterator(NULL);
}

int Rope::size() const{
	return size_;
}

void Rope::print(std::ostream& out) const{
	print(out,root,0);
}

void Rope::print(std::ostream& out, Node* head, int depth) const{
	if(!head){
		return;
	}

	print(out,head->right,depth+1);
	out << std::string(depth*(size_),' ');
	if(is_leaf(head)){
		out << "\"" << head->value << "\"";
	}
	out << " [" << head->weight << "]" << std::endl;
	print(out,head->left,depth+1);
}

void Rope::print_strings(std::ostream& out) const{
	bool first_node = true;
	out << "Strings for rope of size: " << size_ << std::endl;
	for(rope_iterator it = begin(); it!= end(); it++){
		if(!is_leaf(it.ptr_)){
			continue;
		}
		if(!first_node){
			out << "|" << it.value();
		}
		else{
			first_node = false;
			out << it.value();
		}
	}
	std::cout << std::endl;
}

Node* Rope::expose_root() const{
	return root;
}

// Should advance to the next Node using in-order traversal
// It can point at any Node, not just leaves
rope_iterator &rope_iterator::operator++()
{
	if (ptr_->right)
	{ // find the leftmost child of the right node
		ptr_ = ptr_->right;
		while (ptr_->left)
		{
			ptr_ = ptr_->left;
		}
	}
	else
	{ // go upwards along right branches...  stop after the first left
		while (ptr_->parent && ptr_->parent->right == ptr_)
		{
			ptr_ = ptr_->parent;
		}
		ptr_ = ptr_->parent;
	}
	return *this;
}

// Point to the first Node for in-order traversal
rope_iterator Rope::begin() const
{
	if (!root)
		return rope_iterator(NULL);
	Node *p = root;
	while (p->left)
		p = p->left;
	return rope_iterator(p);
}

// Should make the root = p and adjust any member variables
// This should not make a copy of the rope with root at p,
// it should just "steal" the Node*
Rope::Rope(Node *p)
{
	root = p;
	size_ = p->weight;

	// the root node may have right subtrees
	if (p->right)
	{
		size_ += p->right->weight;
	}
}

Rope::~Rope()
{
	this->destroy_rope(root);
	root = NULL;
}

Rope::Rope(const Rope &r)
{
	size_ = r.size_;
	root = this->copy_rope(r.root, NULL);
}

Rope &Rope::operator=(const Rope &r)
{
	if (&r != this)
	{
		this->destroy_rope(root);
		root = this->copy_rope(r.root, NULL);
		size_ = r.size_;
	}
	return *this;
}

Node *Rope::copy_rope(Node *old_root, Node *the_parent)
{
	if (old_root == NULL)
		return NULL;
	Node *answer = new Node();
	answer->value = old_root->value;
	answer->weight = old_root->weight;
	answer->left = copy_rope(old_root->left, answer);
	answer->right = copy_rope(old_root->right, answer);
	answer->parent = the_parent;
	return answer;
}

void Rope::destroy_rope(Node *p)
{
	if (!p)
		return;
	destroy_rope(p->right);
	destroy_rope(p->left);
	delete p;
}

// Get a single character at index i
bool Rope::index(int i, char &c) const
{
	Node *p = root;
	// iterate until reach leaves
	while (p->left || p->right)
	{

		if (i < p->weight)
		{
			p = p->left;
		}
		else
		{
			i -= p->weight;
			p = p->right;
		}
	}
	c = (p->value)[i];
	return true;
}

// Add the other rope (r)'s string to the end of my string
void Rope::concat(const Rope &r)
{
	Node *p = new Node(); // new root
	Node *rhs = copy_rope(r.root, NULL);
	p->left = this->root;
	p->right = rhs;
	p->weight = this->root->weight;

	// possible rhs subtrees
	if (this->root->right)
	{
		p->weight += this->root->right->weight;
	}

	// update size_
	this->size_ += rhs->weight;
	if (rhs->right)
	{
		this->size_ += rhs->right->weight;
	}

	// move old root to new root and change parents
	this->root = p;
	this->root->left->parent = this->root;
	this->root->right->parent = this->root;
}

// Get a substring from index i to index j.
// Includes both the characters at index i and at index j.
// String can be one character if i and j match
// Returns true if a string was returned, and false otherwise
// Function should be written with efficient running time.
bool Rope::report(int i, int j, std::string &s) const
{
	if (i > j)
		return false;
	else
	{
		char c;
		s.clear();
		for (; i <= j; i++)
		{
			this->index(i, c);
			s += c;
		}

		return true;
	}
}

// The first i characters should stay in the current rope, while a new
// Rope (rhs) should contain the remaining size_-i characters.
// A valid split should always result in two ropes of non-zero length.
// If the split would not be valid, this rope should not be changed,
// and the rhs rope should be an empty rope.
// The return value should be this rope (the "left") part of the string
// This function should move the nodes to the rhs instead of making new copies.
Rope &Rope::split(int i, Rope &rhs)
{
	// not valid
	if (i <= 0 || i > this->size_)
	{
		rhs.destroy_rope(rhs.root);
		rhs.size_ = 0;
	}

	// valid
	else
	{
		Node *p = root;
		int index_helper = i;
		// iterate until reach leaves
		while (p->left || p->right)
		{

			if (index_helper < p->weight)
			{
				p = p->left;
			}
			else
			{
				index_helper -= p->weight;
				p = p->right;
			}
		}

		// special situation ---------------------------------
		// when i is in the middle of a node's value
		// when i==12: split me_i into m & e_i
		//          [11]m  [12]e  [13]_  [14]i
		// value_length: 4
		// index_helper==1
		if (index_helper != 0)
		{
			int value_length = p->weight;
			char c;

			// indice the values for new nodes
			std::string tmp_str1, tmp_str2;
			for (int j = index_helper; j > 0; j--)
			{
				this->index(i - j, c);
				tmp_str1 += c;
			}
			for (int j = 0; j < value_length; j++)
			{
				this->index(i + j + index_helper - 1, c);
				tmp_str2 += c;
			}
			// create pointers for two new nodes
			p->left = new Node();
			p->left->value = tmp_str1;
			p->left->weight = index_helper;
			p->left->parent = p;

			p->right = new Node();
			p->right->value = tmp_str2;
			p->right->weight = value_length - index_helper;
			p->right->parent = p;

			p->weight = index_helper; // update weight of the parent node
			p = p->right;			  // move the pointer to the leaf
		}
		// special situation ends---------------------------------

		rhs.root = p;
		rhs.size_ = p->weight; // 2
		p = p->parent;
		p->right->parent = NULL;
		p->right = NULL;
		p = p->parent;

		// concat_no_copy-------------------------------------
		Node *new_start = new Node(); // new root
		new_start->left = rhs.root;
		new_start->right = p->right;
		new_start->weight = rhs.root->weight;

		// possible rhs subtrees
		if (rhs.root->right)
		{
			p->weight += this->root->right->weight;
		}

		// move old root to new root and change parents
		rhs.root = new_start;
		rhs.root->left->parent = rhs.root;
		rhs.root->right->parent = rhs.root;
		p->weight -= rhs.root->weight;
		p->right = NULL;
		// concat end-----------------------------------------

		//        // iterate up to the root
		//        while (p->parent->parent != nullptr){
		//            // break if
		//            if (p->right) {
		//                rhs.concat_no_copy(p->right);
		//                p->weight -= rhs.root->weight;
		//                p->right->parent = NULL;
		//                p->right = NULL;
		//            }
		//
		//            p = p->parent;
		//        }

		// update size of two ropes
		rhs.size_ = this->size_ - i;
		this->size_ = i;
		this->root->weight -= i;
	}
	return rhs;
}