#include "Bootstrap.h"

#include <exception>
#include <iostream>

int main()
{
	Goss::Bootstrap bootstrap{};

    try
    {
        bootstrap.Run();
    }
	catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
