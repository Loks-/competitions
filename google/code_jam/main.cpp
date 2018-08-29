#include "common/base.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int proxy(const std::string& s);

int main(int nargs, char **pargs)
{
	std::string solution_name;
	std::ifstream input_file;
	std::ifstream output_file;
	std::stringstream solution_output, expected_output;
	std::streambuf* cout_backup = 0;
	if (nargs >= 2)
	{
		solution_name = pargs[1];
	}
	else
	{
		std::cout << "Input google code jam solution to run:" << std::endl;
		std::cin >> solution_name;
	}
	if (nargs >= 3)
	{
		input_file.open(pargs[2]);
		std::cin.rdbuf(input_file.rdbuf());
	}
	if (nargs >= 4)
	{
		cout_backup = std::cout.rdbuf(solution_output.rdbuf());
	}
	int solution_results = proxy(solution_name);
	if (!solution_results && (nargs >= 4))
	{
		std::cout.rdbuf(cout_backup);
		output_file.open(pargs[3]);
		expected_output << output_file.rdbuf();
		if (solution_output.str() != expected_output.str())
		{
			std::cout << "Output for " << solution_name << " doesn't match expectation." << std::endl;
			//std::cout << "Solution output:" << std::endl;
			//std::cout << solution_output.str() << std::endl;
			//std::cout << "Expected output:" << std::endl;
			//std::cout << expected_output.str() << std::endl;
			return -2;
		}
	}
	return solution_results;
}
