#pragma once
#include <iostream>
#include <ostream>

#include "Application.h"

#ifdef  GE_PLATFORM_WINDOWS

extern Goss::Application* Goss::CreateApplication();

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
