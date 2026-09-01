#include "node.h"
#include <string>


enum type
{
	weapon,
	armor,
	consumable,
	utility
};


node::node(std::string _name, type _type, float _price, int _quantity)
{
	item_name = _name;
	item_type = _type;
	item_price = _price;
	item_quantity = _quantity;
	prev = nullptr;
	next = nullptr;
}

