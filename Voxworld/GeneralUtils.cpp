#include "GeneralUtils.h"
#include <iostream>
namespace Utils
{
	GeneralUtils::GeneralUtils(void)
	{
	}

	void GeneralUtils::exitFatalError(const char *message) 
	{
		std::cout << message << " ";
		std::cin.get();
		exit(1);
	}

	GeneralUtils::~GeneralUtils(void)
	{
	}
};