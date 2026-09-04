#include "dll_functions.h"
#include "node.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <ShObjIdl.h>

dll_functions::dll_functions() {}

void dll_functions::function_call()
{
	std::string _function;

	// List of accepted inputs for each function
	std::string load_file_inputs[5] = { "l", "lf", "lff", "load file", "load from file" };
	std::string add_item_inputs[5] = { "a", "ad", "add", "add item", "ai" };
	std::string print_inputs[5] = { "p", "pl", "print", "print list", "prnt" };
	std::string remove_item_inputs[9] = { "r", "remove", "d", "del", "delete", "remove item", "delete item", "ri", "di"};

	// Calls a function selected by the user
	std::cout << "Call function (? for function list): \n- ";
	std::getline(std::cin, _function);

	// Cheak if input is a valid funcion
	if (std::find(std::begin(load_file_inputs), std::end(load_file_inputs), _function) != std::end(load_file_inputs))
	{
		load_file();
	}
	else if (std::find(std::begin(add_item_inputs), std::end(add_item_inputs), _function) != std::end(add_item_inputs))
	{
		add_at_index();
	}
	else if (std::find(std::begin(print_inputs), std::end(print_inputs), _function) != std::end(print_inputs))
	{
		print_list();
	}
	else if (std::find(std::begin(remove_item_inputs), std::end(remove_item_inputs), _function) != std::end(remove_item_inputs))
	{
		add_at_index();
	}


}

void dll_functions::load_list(wchar_t* file)
{
	std::ifstream read_file(file);
	bool list_head = true;
	std::string item_name;
	node::type item_type;
	float item_price;
	int item_quantity = NULL;
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
			bool itemExists = false;
			// split line at ,
			while (std::getline(ss, charic, ','))
			{
				itemExists = true;
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

			if (!itemExists) continue;

			// Make node from read data
			if (list_head && item_quantity != NULL)
			{
				n = new node(item_name, item_type, item_price, item_quantity);
				head = n;
				list_head = false;
			}
			else if (item_quantity != NULL)
			{
				tail = new node(item_name, item_type, item_price, item_quantity);
				tail->prev = n;
				n->next = tail;
				n = tail;
			}
			else
			{
				std::cout << "\n!!!!! File Load failed !!!!!" << std::endl;
				std::cout << "file type must be .txt" << std::endl;
				std::cout << "line format must be:" << std::endl;
				std::cout << "Name,Type,Price,Quantity\n\n" << std::endl;
				return;
			}
			item_quantity = NULL;
		}

		// Print inv
		std::cout << "File Loaded successfully!" << std::endl;
		print_list();

	}
}

void dll_functions::load_file()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog* pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			hr = pFileOpen->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem = nullptr;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath = nullptr;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{	
						load_list(pszFilePath);
						CoTaskMemFree(pszFilePath);
					}

					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
}

// Not yet functional
void dll_functions::save_file()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileSaveDialog* pFileSave;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

		if (SUCCEEDED(hr))
		{
			// Show the Save dialog box.
			hr = pFileSave->Show(NULL);

			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						//if (_Texture->getTexture().copyToImage().saveToFile(pszFilePath))
						//{
						//	MessageBoxW(NULL, pszFilePath, L"Saved to: ", MB_OK);
						//	CoTaskMemFree(pszFilePath);
						//}
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
}

void dll_functions::add_at_index()
{
	// get index
	int index = NULL;
	std::string index_input;
	std::string item_name;
	node::type item_type = node::null_type;
	std::string item_type_input;
	float item_price = NULL;
	std::string item_price_input;
	int item_quantity = NULL;
	std::string item_quantity_input;

	// item index
	while (index == NULL)
	{
		bool index_error = 0;
		std::cout << "Existing item to add the new item after (index)\n";
		std::cout << "eg: \"0\" <item> \"1\" <item> \"2\" <item> \"3\"...\n- ";
		getline(std::cin, index_input);
		std::cout << std::endl;
		for (char& c : index_input)
		{
			if (c < 48 || c > 57)
			{
				std::cout << "!! index can only contain intiger charicters\n" << std::endl;
				index_error = 1;
				continue;
			}
		}
		if (!index_error) { index = stoi(index_input); break; }
	}


	std::cout << "Item Name (more than 20 charicters will get cut off)\n- ";
	std::cin >> item_name;
	std::cout << std::endl;

	// item type
	while (item_type == node::null_type)
	{
		std::cout << "item type: Weapon(1), Armour(2), Consumable(3), Utility(4)\n- ";
		std::cin >> item_type_input;
		for (auto& c : item_type_input)
		{
			c = tolower(c);
		}
		std::cout << std::endl;

		if (item_type_input == "weapon" || item_type_input == "1")
		{
			item_type = node::Weapon;
		}
		else if (item_type_input == "armour" || item_type_input == "2")
		{
			item_type = node::Armor;
		}
		else if (item_type_input == "consumable" || item_type_input == "3")
		{
			item_type = node::Consumable;
		}
		else if (item_type_input == "utility" || item_type_input == "4")
		{
			item_type = node::Utility;
		}
		else
		{
			std::cout << "please try again and ensure spelling\nor number entered is valid/correct\n" << std::endl;
		}
	}

	// Price
	while (item_price == NULL)
	{	
		bool has_point = 0;
		bool price_error = 0;
		std::cout << "Item Price: ";
		std::cin >> item_price_input;
		std::cout << std::endl;
		for (char& c : item_price_input)
		{
			if (c >= 48 || c <= 57 || c == '.')
			{
				if (!has_point && c == '.') { has_point = true; }
				else if (c == '.')
				{
					std::cout << "!! price can not contain multiple points(.)\n" << std::endl;
					price_error = 1;
					continue;
				}
			}
			else
			{
				std::cout << "!! price can only contain intiger or decimal point charicters\n" << std::endl;
				price_error = 1;
				continue;
			}
		}
		if (!price_error)
		{
			has_point ? item_price_input += "0" : item_price_input += ".0";
			item_price = stof(item_price_input);
			break;
		}
	}

	// Quantity
	while (item_quantity == NULL)
	{
		bool quantity_error = 0;
		std::cout << "Quantity of Item: ";
		std::cin >> item_quantity_input;
		std::cout << std::endl;
		for (char& c : item_quantity_input)
		{
			if (c < 48 || c > 57)
			{
				std::cout << "!! quantity can only contain intiger charicters\n" << std::endl;
				quantity_error = 1;
				continue;
			}
		}
		if (!quantity_error) { item_quantity = stoi(item_quantity_input); break; }
	}

	// go to index
	n = head;
	for (index; index > 0; --index)
	{	
		if (n != nullptr) { n = n->next; }
	}

	// if list is empty
	if (head == NULL)
	{
		n = new node(item_name, item_type, item_price, item_quantity);
		head = n;
		tail = n;
	}
	// If index last (adding to very end of list)
	else if (n == nullptr)
	{
		n = tail;
		n->next = new node(item_name, item_type, item_price, item_quantity);
		n->next->prev = n;
		tail = n->next;
	}
	// If index 0 (adding to very start of list)
	else if (n->prev == nullptr)
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

void dll_functions::remove_item()
{
	// vars
	int index = NULL;
	std::string index_input;
	std::string item_name;
	node::type item_type = node::null_type;
	std::string item_type_input;
	float item_price = NULL;
	std::string item_price_input;
	int item_quantity = NULL;
	std::string item_quantity_input;

	// item index
	while (index == NULL)
	{
		bool index_error = 0;
		std::cout << "Existing item to add the new item after (index)\n";
		std::cout << "eg: \"0\" <item> \"1\" <item> \"2\" <item> \"3\"...\n- ";
		std::cin >> index_input;
		std::cout << std::endl;
		for (char& c : index_input)
		{
			if (c < 48 || c > 57)
			{
				std::cout << "!! index can only contain intiger charicters\n" << std::endl;
				index_error = 1;
				continue;
			}
		}
		if (!index_error) { index = stoi(index_input); }
	}


	std::cout << "Item Name (more than 20 charicters will get cut off)\n- ";
	std::cin >> item_name;
	std::cout << std::endl;

	// item type
	while (item_type == node::null_type)
	{
		std::cout << "item type: Weapon(1), Armour(2), Consumable(3), Utility(4)\n- ";
		std::cin >> item_type_input;
		for (auto& c : item_type_input)
		{
			c = tolower(c);
		}
		std::cout << std::endl;

		if (item_type_input == "weapon" || item_type_input == "1")
		{
			item_type = node::Weapon;
		}
		else if (item_type_input == "armour" || item_type_input == "2")
		{
			item_type = node::Armor;
		}
		else if (item_type_input == "consumable" || item_type_input == "3")
		{
			item_type = node::Consumable;
		}
		else if (item_type_input == "utility" || item_type_input == "4")
		{
			item_type = node::Utility;
		}
		else
		{
			std::cout << "please try again and ensure spelling\nor number entered is valid/correct\n" << std::endl;
		}
	}

	// Price
	while (item_price == NULL)
	{
		bool has_point = 0;
		bool price_error = 0;
		std::cout << "Item Price: ";
		std::cin >> item_price_input;
		std::cout << std::endl;
		for (char& c : item_price_input)
		{
			if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0' || c == '.' || c == ',')
			{
				if (!has_point && c == '.' || c == ',') { has_point = true; }
				else if (c == '.' || c == ',')
				{
					std::cout << "!! price can not contain multiple points(.)\n" << std::endl;
					price_error = 1;
					continue;
				}
			}
			else
			{
				std::cout << "!! price can only contain intiger or decimal point charicters\n" << std::endl;
				price_error = 1;
				continue;
			}
		}
		if (!price_error) { item_price = stof(item_price_input); }
	}

	// Quantity
	while (item_quantity == NULL)
	{
		bool quantity_error = 0;
		std::cout << "Quantity of Item: ";
		std::cin >> item_quantity_input;
		std::cout << std::endl;
		for (char& c : item_quantity_input)
		{
			if (!(c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0'))
			{
				std::cout << "!! quantity can only contain intiger charicters\n" << std::endl;
				quantity_error = 1;
				continue;
			}
		}
		if (!quantity_error) { item_quantity = stoi(item_quantity_input); }
	}

	// go to index
	n = head;
	for (index; index > 0; --index)
	{
		if (n->next != nullptr) { n = n->next; }
	}
}

void dll_functions::print_list()
{
	// inv header
	std::cout << "*---------------------*--------------*---------*----------*" << std::endl;
	std::cout << "| Name\033[23G| Type\033[38G| Price\033[48G| Quantity |" << std::endl;
	std::cout << "|---------------------|--------------|---------|----------|" << std::endl;

	// print inv
	if (head != NULL)
	{
		n = head;
		std::cout << "| " << n->item_name << "\033[23G| " << type_to_string(n->item_type) << "\033[38G| " << n->item_price << "\033[48G| " << n->item_quantity << "\033[59G|" << std::endl;
		while (n->next != nullptr)
		{
			n = n->next;
			std::cout << "| " << n->item_name << "\033[23G| " << type_to_string(n->item_type) << "\033[38G| " << n->item_price << "\033[48G| " << n->item_quantity << "\033[59G|" << std::endl;
		}
		std::cout << "*---------------------*--------------*---------*----------*" << std::endl;
	}
	else
	{
		std::cout << "|                     |              |         |          |" << std::endl;
		std::cout << "*---------------------*--------------*---------*----------*" << std::endl;
		std::cout << "\nUse load list(l) or add item(a)\nto see items in this screen\n" << std::endl;
	}

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