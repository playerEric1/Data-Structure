/*
Do not change this file, Submitty will ignore any changes you make
to this file.
*/

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
