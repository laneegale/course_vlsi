#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <bits/stdc++.h>

typedef std::vector<std::vector<double>> SparseMatrix;
typedef std::map<int, std::vector<int>> Gatemap;
typedef std::map<int, std::vector<std::vector<double>>> Padmap;

struct Point
{
    double x;
    double y;
};

struct Netlist
{
    // Given
    int num_of_gates;
    int num_of_nets;

    // Derived
    Gatemap net_to_gates;
    std::map<int, std::vector<double>> net_to_pads;
    Gatemap gate_to_gates;
    Padmap gate_to_pads;
    std::vector<int> gate_id;
    std::map<int, Point> gate_to_coord;
};

#endif