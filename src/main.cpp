#include "repl.h"
#include <iostream>

int main()
{
	std::cout << std::unitbuf;
	std::cerr << std::unitbuf;
	repl();
	return 0;
}
