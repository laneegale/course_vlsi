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

/**
 * Read data from an input file
 *
 * For .gird file
 * The first line has 4 numbers: X gridsize, Y gridsize, BendPenalty, ViaPenalty
 * The following Y lines have X numbers each, indicating the cost of each cell
 * 
 * For .nl file
 * The first line has 1 number: number of wires
 * For the next number of wires lines, each line has 7 numbers: netID, pin1 layer id, pin1 x, pin1 y, pin2 layer id, pin2 x, pin2 y
 * 
 * for layer id: 0 signals the end of list, 1 means layer 1, 2 means layer 2, 3 means this is a via
 *
 * @param values Container whose values are summed.
 * @return sum of `values`, or 0.0 if `values` is empty.
 */
Raw_data read_data(const std::string &filename);

#endif