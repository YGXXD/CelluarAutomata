#include <iostream>
#include "Core/Application.h"
#include <unistd.h>
#include <SDL.h>

int main()
{
	try 
	{
		Application::Get().Create();

		Application::Get().Run();

		char path[128];
		getcwd (path,128);
		std::cout << path << std::endl;

		Application::Get().Destroy();
		return 0;
	}
   	catch (const char*& e) 
	{
		std::cout << e << std::endl;
		return 0;
	}
}
