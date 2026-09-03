#pragma once
#include <fstream>

class node
{
public:
	enum type
	{
		Weapon,
		Armor,
		Consumable,
		Utility,
		null_type
	};

	std::string item_name;
	type item_type;
	float item_price;
	int item_quantity;
	node* prev;
	node* next;
	

	node(std::string _name, type _type, float _price, int _quantity);

};

