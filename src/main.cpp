#include <unistd.h>
#include "Core/Application.h"
#include "Birth2025/Birth2025World.h"

int main()
{
	try 
	{
		Application::Get().Create();

		Application::Get().Run<Birth2025World>();

		Application::Get().Destroy();
		return 0;
	}
   	catch (const std::string& e) 
	{
		Application::Get().Destroy();

		char path[128];
		getcwd (path,128);
		std::cout << "当前工作路径:" << path << std::endl;
		std::cout << "异常:" << e << std::endl;

		return 0;
	}
}
