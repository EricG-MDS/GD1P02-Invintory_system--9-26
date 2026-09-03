#pragma once
#include "node.h"
class dll_functions
{
public:
	dll_functions();
	node* head = NULL;
	node* tail;
	node* n;

	std::string type_to_string(node::type _type);

	void function_call();

	void load_file();

	void save_file();

	void load_list(wchar_t* _file);

	void add_at_index();

	void remove_item();

	void print_list();

};

