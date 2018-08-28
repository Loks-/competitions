#include "common/base.h"

#include <iostream>
#include <string>

int proxy(const std::string& s);

int main(int nargs, char **pargs)
{
	std::string s;
	if (nargs <= 1)
	{
		std::cout << "Input google code jam solution to run:" << std::endl;
		std::cin >> s;
	}
	else
		s = pargs[1];
	return proxy(s);
}
