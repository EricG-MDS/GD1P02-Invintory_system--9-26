#include "dll_functions.h"
#include "node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

dll_functions::dll_functions() {}

void dll_functions::load_list(std::string file)
{
	std::ifstream read_file(file);
	bool list_head = true;
	std::string item_name;
	node::type item_type;
	float item_price;
	int item_quantity;
	std::string charic;

	// Check if file opened / exists
	if (read_file.is_open())
	{
		// Read file line by line
		std::string line;
		while (std::getline(read_file, line))
		{
			std::stringstream ss(line);
			int var_num = 0;

			// split line at ,
			while (std::getline(ss, charic, ','))
			{
				if (var_num == 0)
				{
					item_name = charic;
				}
				else if (var_num == 1)
				{
					if (charic == "Weapon")
					{
						item_type = node::Weapon;
					}
					else if (charic == "Armor")
					{
						item_type = node::Armor;
					}
					else if (charic == "Consumable")
					{
						item_type = node::Consumable;
					}
					else if (charic == "Utility")
					{
						item_type = node::Utility;
					}
				}
				else if (var_num == 2)
				{
					item_price = std::stof(charic);
				}
				else
				{
					item_quantity = std::stoi(charic);
				}
				var_num++;
			}

			// Make node from read data
			if (list_head)
			{
				n = new node(item_name, item_type, item_price, item_quantity);
				head = n;
				list_head = false;
			}
			else
			{
				tail = new node(item_name, item_type, item_price, item_quantity);
				tail->prev = n;
				n->next = tail;
				n = tail;
			}
		}

		// Print inv
		print_list();

	}
}

void dll_functions::add_at_index()
{
	// get index
	int index;
	std::string item_name;
	node::type item_type;
	std::string item_type_input;
	float item_price;
	int item_quantity;

	std::cout << "Index: ";
	std::cin >> index; // ADD INT CHECK
	std::cout << std::endl;

	std::cout << "Name: ";
	std::cin >> item_name;
	std::cout << std::endl;

	std::cout << "type: ";
	std::cin >> item_type_input;
	std::cout << std::endl;

	if (item_type_input == "Weapon" || item_type_input == "1")
	{
		item_type = node::Weapon;
	}
	else if (item_type_input == "Armor" || item_type_input == "2")
	{
		item_type = node::Armor;
	}
	else if (item_type_input == "Consumable" || item_type_input == "3")
	{
		item_type = node::Consumable;
	}
	else if (item_type_input == "Utility" || item_type_input == "4")
	{
		item_type = node::Utility;
	}

	std::cout << "Price: ";
	std::cin >> item_price;
	std::cout << std::endl;

	std::cout << "quantity: ";
	std::cin >> item_quantity;
	std::cout << std::endl;

	// go to index
	n = head;
	for (index; index > 0; --index)
	{
		n = n->next;
	}

	if (n->prev == nullptr)
	{
		n->prev = new node(item_name, item_type, item_price, item_quantity);
		n->prev->next = n;
		head = n->prev;
	}
	else
	{
		// make new node
		n->prev->next = new node(item_name, item_type, item_price, item_quantity);
		n->prev->next->prev = n->prev;
		n->prev->next->next = n;
		n = n->prev->next;
		n->next->prev = n;
	}
}

void dll_functions::print_list()
{
	// inv header
	std::cout << "*---------------------*--------------*---------*----------*" << std::endl;
	std::cout << "| Name\033[23G| Type\033[38G| Price\033[48G| Quantity |" << std::endl;
	std::cout << "|---------------------|--------------|---------|----------|" << std::endl;

	// print inv
	n = head;
	std::cout << "| " << n->item_name << "\033[23G| " << type_to_string(n->item_type) << "\033[38G| " << n->item_price << "\033[48G| " << n->item_quantity << "\033[59G|" << std::endl;
	while (n->next != nullptr)
	{
		n = n->next;
		std::cout << "| " << n->item_name << "\033[23G| " << type_to_string(n->item_type) << "\033[38G| " << n->item_price << "\033[48G| " << n->item_quantity << "\033[59G|" << std::endl;
	}
	std::cout << "*---------------------*--------------*---------*----------*" << std::endl;

}

std::string dll_functions::type_to_string(node::type _type)
{
	if (_type == 0)
	{
		return "Weapon";
	}
	else if (_type == 1)
	{
		return "Armor";
	}
	else if (_type == 2)
	{
		return "Consumable";
	}
	else if (_type == 3)
	{
		return "Utility";
	}
}