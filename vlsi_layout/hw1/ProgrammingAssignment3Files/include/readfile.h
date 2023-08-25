#ifndef _READFILE_H_
#define _READFILE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

#include "./data.h"

std::vector<std::string> splitString(const std::string &input, const char &delimiter);

std::string removeSpecialCharacters(const std::string &input);

std::vector<std::string> clean_tokens(const std::vector<std::string> &tokens);

std::vector<std::string> process_line(const std::string &line);

Netlist read_data(const std::string &filename);

#endif