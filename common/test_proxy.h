#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int proxy(const std::string& s);

inline int test_proxy(const std::string& solution, const std::string& input_file_name, const std::string& test_file_name)
{
	std::ifstream input_file, test_file;
	std::stringstream solution_output, expected_output;
	std::streambuf* cout_backup = 0;
	if (!input_file_name.empty())
	{
		input_file.open(input_file_name);
		std::cin.rdbuf(input_file.rdbuf());
	}
	if (!test_file_name.empty())
		cout_backup = std::cout.rdbuf(solution_output.rdbuf());
	int solution_results = proxy(solution);
	if (!solution_results && !test_file_name.empty())
	{
		std::cout.rdbuf(cout_backup);
		test_file.open(test_file_name);
		expected_output << test_file.rdbuf();
		if (solution_output.str() != expected_output.str())
		{
			std::cout << "Output for " << solution << " doesn't match expectation." << std::endl;
			return -2;
		}
	}
	return solution_results;
}
