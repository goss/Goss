#pragma once

#include "Log.h"
#include "Application.h"

// std
#include <iostream>
#include <ostream>

#ifdef  GE_PLATFORM_WINDOWS


int main(int argc, char** argv)
{
	Goss::Application* const application = Goss::CreateApplication();

    try
    {
        application->Run();
    }
	catch (const std::exception& e) 
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete application;
    return EXIT_SUCCESS;
}
#endif
