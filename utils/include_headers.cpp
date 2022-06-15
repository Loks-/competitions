#include "common/base.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

static std::unordered_set<std::string> used_files, used_stl_files;

static std::string JoinPath(const std::string& sdir, const std::string& sfile) {
  assert((sdir.back() == '/') || (sdir.back() == '\\'));
  size_t nposl1 = sdir.find_last_of('/'), nposl2 = sdir.find_last_of('\\');
  size_t nposl = (nposl1 == std::string::npos)   ? nposl2
                 : (nposl2 == std::string::npos) ? nposl1
                                                 : std::max(nposl1, nposl2);
  assert(nposl != std::string::npos);
  size_t nposr = 0;
  for (;;) {
    if ((nposl == 0) || (sdir[nposl - 1] == '.')) break;
    if (sfile.substr(nposr, 3) != "../") break;
    size_t nposl1 = sdir.find_last_of('/', nposl - 1),
           nposl2 = sdir.find_last_of('\\', nposl - 1);
    size_t nposl_new = (nposl1 == std::string::npos) ? nposl2
                       : (nposl2 == std::string::npos)
                           ? nposl1
                           : std::max(nposl1, nposl2);
    if (nposl_new == std::string::npos) break;
    nposl = nposl_new;
    nposr += 3;
  }
  return sdir.substr(0, nposl + 1) + sfile.substr(nposr);
}

static void AddFile(const std::string& input_filename, std::ofstream& output,
                    bool ignore_local, bool silent = false) {
  bool mode_leetcode = false;
  std::ifstream input(input_filename);
  std::string line;
  while (std::getline(input, line)) {
    if (line == "#include \"leetcode/test.h\"") {
      mode_leetcode = true;
      continue;
    }
    if (line == "#pragma once") {
      if (used_files.find(input_filename) != used_files.end()) return;
      used_files.insert(input_filename);
      continue;
    }
    if (line.substr(0, 8) == "#include") {
      size_t npos_include = 8;
      for (; (npos_include < line.size()) && isspace(line[npos_include]);)
        ++npos_include;
      assert(npos_include < line.size());
      if (line[npos_include] == '"') {
        size_t npos_include_end = line.find_last_of('"');
        assert(npos_include_end > npos_include);
        std::string new_header_file =
            line.substr(npos_include + 1, npos_include_end - npos_include - 1);
        if (ignore_local) {
          size_t hnpos1 = new_header_file.find_last_of('\\');
          size_t hnpos2 = new_header_file.find_last_of('/');
          if ((hnpos1 == std::string::npos) && (hnpos2 == std::string::npos)) {
            output << line << std::endl;
            continue;
          }
        }
        std::string current_dir = ".\\";
        size_t npos1 = input_filename.find_last_of('\\');
        size_t npos2 = input_filename.find_last_of('/');
        size_t npos = (npos1 == std::string::npos)   ? npos2
                      : (npos2 == std::string::npos) ? npos1
                                                     : std::max(npos1, npos2);
        if (npos != std::string::npos)
          current_dir = input_filename.substr(0, npos + 1);

        std::string new_file = (new_header_file.substr(0, 7) == "common/")
                                   ? new_header_file
                                   : JoinPath(current_dir, new_header_file);
        if (!silent) {
          std::cout << "Include file found:" << std::endl;
          std::cout << "\t[" << input_filename << "] [" << new_header_file
                    << "] [" << new_file << "]" << std::endl;
        }
        AddFile(new_file, output, false, silent);
        continue;
      } else if (line[npos_include] == '<') {
        while (line.back() == ' ') line.pop_back();
        assert(line.back() == '>');
        if (used_stl_files.find(line) != used_stl_files.end()) continue;
        used_stl_files.insert(line);
      }
    }
    if (mode_leetcode) {
      if (line == "namespace {") continue;
      if (line == "}  // namespace") break;
      if (line.substr(0, 14) == "class Solution") {
        output << "class Solution {" << std::endl;
        continue;
      }
    }
    if (line.substr(0, 8) == "int main") {
      output << "int main() {" << std::endl;
      continue;
    }
    if (line == "static const bool local_run = true;") {
      output << "static const bool local_run = false;" << std::endl;
      continue;
    }
    output << line << std::endl;
  }
}

int main(int nargs, char** pargs) {
  std::string input_filename, output_filename;
  if (nargs <= 1)
    std::cin >> input_filename;
  else
    input_filename = pargs[1];
  if (nargs <= 2)
    output_filename = "utils/current_solution.cpp";
  else
    output_filename = pargs[2];
  std::ofstream output(output_filename);
  AddFile(input_filename, output, false);
  return 0;
}
