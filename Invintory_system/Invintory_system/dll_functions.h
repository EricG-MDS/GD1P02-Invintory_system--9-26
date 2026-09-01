#pragma once
#include "node.h"
class dll_functions
{
public:
	dll_functions();
	node* head;
	node* tail;
	node* n;

	std::string type_to_string(node::type _type);

	void load_list(std::string _file);

	void add_at_index();

	void print_list();

};

