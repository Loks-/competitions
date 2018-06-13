#include "base.h"

// TODO:
//   Support Unix directories

static unordered_set<string> used_files;

static string MergePath(const string& sdir, const string& sfile)
{
	assert((sdir.back() == '/') || (sdir.back() == '\\'));
	size_t nposl1 = sdir.find_last_of('/'), nposl2 = sdir.find_last_of('\\');
	size_t nposl = (nposl1 == string::npos) ? nposl2 : (nposl2 == string::npos) ? nposl1 : max(nposl1, nposl2);
	assert(nposl != string::npos);
	size_t nposr = 0;
	for (;;)
	{
		if ((nposl == 0) || (sdir[nposl - 1] == '.'))
			break;
		if (sfile.substr(nposr, 3) != "../")
			break;
		size_t nposl1 = sdir.find_last_of('/', nposl - 1), nposl2 = sdir.find_last_of('\\', nposl - 1);
		size_t nposl_new = (nposl1 == string::npos) ? nposl2 : (nposl2 == string::npos) ? nposl1 : max(nposl1, nposl2);
		if (nposl_new != string::npos)
		{
			nposl = nposl_new;
			nposr += 3;
		}
		else
			break;
	}
	return sdir.substr(0, nposl + 1) + sfile.substr(nposr);
}

static void AddFile(const string& input_filename, ofstream& output, bool ignore_local, bool silent = false)
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
				if (ignore_local)
				{
					size_t hnpos1 = new_header_file.find_last_of('\\');
					size_t hnpos2 = new_header_file.find_last_of('/');
					if ((hnpos1 == string::npos) && (hnpos2 == string::npos))
					{
						output << line << endl;
						continue;
					}
				}
				string current_dir = ".\\";
				size_t npos1 = input_filename.find_last_of('\\');
				size_t npos2 = input_filename.find_last_of('/');
				size_t npos = (npos1 == string::npos) ? npos2 : (npos2 == string::npos) ? npos1 : max(npos1, npos2);
				if (npos != string::npos)
				{
					current_dir = input_filename.substr(0, npos + 1);
				}
				string new_file = (new_header_file.substr(0, 7) =="common/") ? new_header_file : MergePath(current_dir, new_header_file);
				if (!silent)
				{
					cout << "Include file found:" << endl;
					cout << "\t[" << input_filename << "] [" << new_header_file << "] [" << new_file << "]" << endl;
				}
				AddFile(new_file, output, false, silent);
				continue;
			}
		}
		if (line.substr(0, 8) == "int main")
		{
			output << "int main()" << endl;
			continue;
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
	AddFile(input_filename, output, true);
	return 0;
}
