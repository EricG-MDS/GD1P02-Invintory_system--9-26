#include <iostream>

// Header includes
#include "node.h"
#include "dll_functions.h"


int main()
{   
    dll_functions dll;
    
    dll.load_list("Inventory-1.txt");
    dll.add_at_index();
    dll.print_list();

}
