#include <iostream>
#include <unistd.h>
#include <SDL.h>
#include "Core/Application.h"
#include "Cauto/NaSchWorld.h"
#include "Cauto/Wolfram184World.h"
#include "Cauto/BMLWorld.h"

int main()
{
	try 
	{
		Application::Get().Create<BMLWorld>();

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
