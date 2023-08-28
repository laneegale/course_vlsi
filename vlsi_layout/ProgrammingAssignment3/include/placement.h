#ifndef _PLACEMENT_H_
#define _PLACEMENT_H_

#include <bits/stdc++.h>

#include "./readfile.h"
#include "./solver.h"
#include "./data.h"

std::tuple<SparseMatrix, std::vector<double>, std::vector<double>> generate_matrix(const Netlist &net_info, const Point &lower_left, const Point &upper_right);

// Solve matrix equation Ax = b
std::vector<double> solve_matrix_equation(const SparseMatrix &a_matrix, const std::vector<double> &b_vector, const int &matrix_size);

// Calculate placement from net info
std::pair<std::vector<double>, std::vector<double>> calculate_placement(const Netlist &net_info, const Point &lower_left, const Point &upper_right);

// Sort the netlist by x_coord, then by y_coord, lastly by gate_id
std::vector<int> sort_gates(const Netlist &net_info, const int &direction);

// Split a netlist into half, need a direction. 0 for vertical, 1 for horizontal
std::pair<Netlist, Netlist> split_netlist(const Netlist &net_info, const int &direction, const int &matrix_size);

// Split a boundry into half
vector<Point> split_region(const Point &lower_left, const Point &upper_right, const int &direction);

// Generate the placement result and write into a file
void placement(Netlist &net_info, const int &cur_iter, const int &max_iter, const Point &bottom_left, const Point &upper_right, const int &direction);

#endif