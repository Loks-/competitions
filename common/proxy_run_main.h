#pragma once

#include "proxy_run_base.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

inline int proxy_run(const std::string& solution)
{
	auto it = solutions_map.find(solution);
	if (it != solutions_map.end())
		return it->second();
	std::cout << "Unknown solution name" << std::endl;
	return -2;
}

inline int proxy_run(const std::string& solution, const std::string& input_file_name)
{
	if (input_file_name.empty())
		return proxy_run(solution);
	std::ifstream input_file(input_file_name);
	auto cin_backup = std::cin.rdbuf(input_file.rdbuf());
	int solution_results = proxy_run(solution);
	std::cin.rdbuf(cin_backup);
	return solution_results;
}

inline int proxy_test_one(const std::string& solution, const std::string& input_file_name, const std::string& test_file_name)
{
	std::stringstream solution_output, expected_output;
	auto cout_backup = std::cout.rdbuf(solution_output.rdbuf());
	int solution_results = proxy_run(solution, input_file_name);
	if (solution_results)
		return solution_results;
	std::cout.rdbuf(cout_backup);
	std::string sraw = solution_output.str(), sfixed;
	sfixed.reserve(sraw.size());
	for (unsigned i = 0; i < sraw.size(); ++i)
	{
		if ((sraw[i] == ' ') && ((sraw[i + 1] == ' ') || (sraw[i + 1] == '\n')))
			continue;
		sfixed.push_back(sraw[i]);
	}
	std::ifstream test_file(test_file_name);
	expected_output << test_file.rdbuf();
	if (sfixed != expected_output.str())
	{
		std::cout << "Output for " << solution << " doesn't match expectation." << std::endl;
		return -3;
	}
	return 0;
}

int proxy_auto(const std::string& solution, const std::string& input_file_name, const std::string& test_file_name)
{
	return test_file_name.empty() ? proxy_run(solution, input_file_name) : proxy_test_one(solution, input_file_name, test_file_name);
}
