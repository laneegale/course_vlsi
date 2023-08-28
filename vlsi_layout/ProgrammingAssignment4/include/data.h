#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <bits/stdc++.h>

typedef std::vector<std::vector<std::vector<int>>> Costgrid;

// Define a type with template of 2D vector
template <typename T>
using Vec3D = std::vector<std::vector<std::vector<T>>>;

template <typename T>
using Vec2D = std::vector<std::vector<T>>;

template <typename T>
using Vec1D = std::vector<T>;


struct Point
{
    int layer;
    int x;
    int y;

    
    bool operator==(const Point &p) const
    {
        return (layer == p.layer && x == p.x && y == p.y);
    };

    bool operator!=(const Point &p) const
    {
        return !(layer == p.layer && x == p.x && y == p.y);
    };
};


struct Wire
{
    int netID;
    Point pin1;
    Point pin2;
};

typedef std::vector<Wire> WireVec;

struct Grid_input
{
    int width;
    int height;
    Costgrid cost_grid;
};

struct Nl_input
{
    int num_wires;
    WireVec wires_info;
};

struct Raw_data
{
    Grid_input grid;
    Nl_input nl;
};

struct Routing_grid
{   
    // Store the predessors of each node
    Vec3D<std::string> pred;
    // Store the visited nodes
    Vec3D<bool> visited;
    Vec3D<int> pathcost;
};

struct Wave_unit
{
    Point point;
    int pathcost;
    std::string pred;

    bool operator<(const Wave_unit &w) const
    {
        return pathcost < w.pathcost;
    };

    bool operator>(const Wave_unit &w) const
    {
        return pathcost > w.pathcost;
    };

    bool operator==(const Wave_unit &w) const
    {
        return (pathcost == w.pathcost);
    };

    bool operator!=(const Wave_unit &w) const
    {
        return (pathcost != w.pathcost);
    };

    bool operator<=(const Wave_unit &w) const
    {
        return (pathcost <= w.pathcost);
    };

    bool operator>=(const Wave_unit &w) const
    {
        return (pathcost >= w.pathcost);
    };
};

#endif