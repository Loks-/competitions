#include "base.h"

// TODO:
//   Support Unix directories

static unordered_set<string> used_files;

static void AddFile(const string& input_filename, ofstream& output, bool silent = false)
{
	ifstream input(input_filename);
	string line;
	while (getline(input, line))
	{
		if (line == "#pragma once")
		{
			if (used_files.find(input_filename) != used_files.end())
				return;
			used_files.insert(input_filename);
			continue;
		}
		if (line.substr(0, 8) == "#include")
		{
			size_t npos_include = 8;
			for (; (npos_include < line.size()) && isspace(line[npos_include]); ++npos_include);
			assert(npos_include < line.size());
			if (line[npos_include] == '"')
			{
				size_t npos_include_end = line.find_last_of('"');
				assert(npos_include_end > npos_include);
				string new_header_file = line.substr(npos_include + 1, npos_include_end - npos_include - 1);
				string current_dir = ".\\";
				size_t npos = input_filename.find_last_of('\\');
				if (npos != string::npos)
				{
					current_dir = input_filename.substr(0, npos + 1);
				}
				string new_file = current_dir + new_header_file;
				if (!silent)
				{
					cout << "Include file found:" << endl;
					cout << "\t[" << input_filename << "] [" << new_header_file << "] [" << new_file << "]" << endl;
				}
				AddFile(new_file, output);
				continue;
			}
		}
		output << line << endl;
	}
}

int main(int nargs, char **pargs)
{
	string input_filename, output_filename;
	if (nargs <= 1)
		cin >> input_filename;
	else
		input_filename = pargs[1];
	if (nargs <= 2)
		output_filename = "common\\current_solution.inl";
	else
		output_filename = pargs[2];
	ofstream output(output_filename);
	AddFile(input_filename, output);
	return 0;
}
