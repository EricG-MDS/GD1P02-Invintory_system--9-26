#include <iostream>

// Header includes
#include "node.h"
#include "dll_functions.h"


int main()
{   
    // Acsessor object for double linked list functions
    dll_functions dll;

    // Start screen title
    std::cout << "########################" << std::endl;
    std::cout << "### Inventory System ###" << std::endl;
    std::cout << "########################\n\n----\n" << std::endl;

    while (true)
    {
        dll.function_call();
    }
}
