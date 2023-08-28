#include "../include/router.h"

Point get_point_from_pred(const Point &p, std::string dir)
{
    // if dir is not in the set {N, S, E, W, U, D}, exit with error
    if (dir != "N" && dir != "S" && dir != "E" && dir != "W" && dir != "U" && dir != "D")
    {
        std::cerr << "ERROR FROM router.cpp:: get_point_from_pred(): Invalid direction " << dir << std::endl;
        exit(1);
    }

    // Get the point from the predessors
    Point pred_point = p;
    if (dir == "N")
    {
        pred_point.y++;
    }
    else if (dir == "S")
    {
        pred_point.y--;
    }
    else if (dir == "E")
    {
        pred_point.x++;
    }
    else if (dir == "W")
    {
        pred_point.x--;
    }
    else if (dir == "U")
    {
        pred_point.layer++;
    }
    else if (dir == "D")
    {
        pred_point.layer--;
    }

    return pred_point;
}

bool point_is_valid(const Raw_data &raw_data, const Routing_grid &routing_grid, const Point& next_pt)
{
    if (raw_data.grid.cost_grid[next_pt.layer][next_pt.y][next_pt.x] == -1)
    {   
        return false;
    };
    if (routing_grid.visited[next_pt.layer][next_pt.y][next_pt.x])
    {
        return false;
    };
    return true;
};

Vec1D<Point> rout_single_wire(const Raw_data &raw_data, Wire wire, Routing_grid &ref_routing_grid)
{
    // Deep copy the ref_routing_grid
    Routing_grid routing_grid = ref_routing_grid;

    // Print the wire info
    // std::cout << "Routing wire " << wire.netID << " from (" << wire.pin1.layer << ", " << wire.pin1.x << ", " << wire.pin1.y << ") to (" << wire.pin2.layer << ", " << wire.pin2.x << ", " << wire.pin2.y << ")" << std::endl;

    Point src = wire.pin1;
    Point dst = wire.pin2;

    if (src == dst)
    {
        return Vec1D<Point>{src};
    }

    // Init a min heap to store the wavefront
    std::priority_queue<Wave_unit, Vec1D<Wave_unit>, std::greater<Wave_unit>> wavefront;
    // Push src cell to wavefront, index it with the cost
    int cost = raw_data.grid.cost_grid[src.layer][src.y][src.x];
    wavefront.push(Wave_unit{src, cost, ""});
    routing_grid.visited[src.layer][src.y][src.x] = true;

    // while (!routing_grid.visited[dst.layer][dst.x][dst.y])
    while (true)
    {
        // If wavefront is empty, return
        if (wavefront.empty())
        {
            return Vec1D<Point>{};
        }
        // Pop the cell with the lowest cost
        Wave_unit curr = wavefront.top();
        wavefront.pop();

        // curr is the dst cell, found the path, backtrace and clean up
        if (curr.point == dst)
        {
            // Backtrace the path
            Vec1D<Point> path;
            Point curr_point = curr.point;
            while (curr_point != src)
            {
                // Push to path
                path.push_back(curr_point);
                // Mark the cell as visited in ref_routing_grid
                ref_routing_grid.visited[curr_point.layer][curr_point.y][curr_point.x] = true;
                // Get the predessors of the cell
                std::string pred = routing_grid.pred[curr_point.layer][curr_point.y][curr_point.x];
                curr_point = get_point_from_pred(curr_point, pred);
            }
            path.push_back(curr_point);
            ref_routing_grid.visited[curr_point.layer][curr_point.y][curr_point.x] = true;
            return path;
        }

        // For each neighbor of curr, push to wavefront
        Vec1D<Point> neighbors;
        Vec1D<std::string> dirs;

        // Write a for loop for i in {S, N, W, E} cout i
        for (int i = 0; i < 4; i++)
        {   
            Point next_pt;
            std::string dir;
            switch (i)
            {
            case 0: //S
                next_pt = Point{curr.point.layer, curr.point.x, curr.point.y + 1};
                dir = "S";
                break;
            case 1:
                next_pt = Point{curr.point.layer, curr.point.x, curr.point.y - 1};
                dir = "N";
                break;
            case 2:
                next_pt = Point{curr.point.layer, curr.point.x + 1, curr.point.y};
                dir = "W";
                break;
            case 3:
                next_pt = Point{curr.point.layer, curr.point.x - 1, curr.point.y};
                dir = "E";
                break;
            
            default:
                // exit with error
                std::cerr << "ERROR FROM router.cpp:: rout_single_wire(): Invalid direction " << dir << std::endl;
                break;
            }

            // check if next_pt is out of bound
            if (next_pt.x < 0 || next_pt.x >= raw_data.grid.width || next_pt.y < 0 || next_pt.y >= raw_data.grid.height)
            {
                continue;
            }

            // check if the next_pt is blocked or visited
            if (point_is_valid(raw_data, routing_grid, next_pt))
            {
                neighbors.push_back(next_pt);
                dirs.push_back(dir);
            }


        }

        // For each neighbor, set as visited, calculate new pathcost, mark the pred map, finally push to wavefront
        for (int i = 0; i < neighbors.size(); i++)
        {
            // Set as visited
            routing_grid.visited[neighbors[i].layer][neighbors[i].y][neighbors[i].x] = true;
            // Calculate new pathcost
            int new_pathcost = curr.pathcost + raw_data.grid.cost_grid[neighbors[i].layer][neighbors[i].y][neighbors[i].x];
            // Mark the pred map
            routing_grid.pred[neighbors[i].layer][neighbors[i].y][neighbors[i].x] = dirs[i];
            // Push to wavefront
            wavefront.push(Wave_unit{neighbors[i], new_pathcost, dirs[i]});
        }
    }

    return Vec1D<Point>{};
}

Vec2D<Point> maze_router(const Raw_data &raw_data)
{
    // Get the reference data
    int width = raw_data.grid.width;
    int height = raw_data.grid.height;
    Costgrid cost_grid = raw_data.grid.cost_grid;
    Nl_input nl = raw_data.nl;

    // Init the routing grid
    Routing_grid routing_grid;
    // Init the predessors grid
    routing_grid.pred = Vec3D<std::string>(2, Vec2D<std::string>(height, Vec1D<std::string>(width, "")));
    // Init the visited grid
    routing_grid.visited = Vec3D<bool>(2, Vec2D<bool>(height, Vec1D<bool>(width, false)));

    Vec2D<Point> results;
    for (int i = 0; i < nl.num_wires; i++)
    {
        Vec1D<Point> temp = rout_single_wire(raw_data, nl.wires_info[i], routing_grid);
        results.push_back(temp);

        // cout the path
        // std::cout << "Path: ";
        // for (int j = 0; j < temp.size(); j++)
        // {
        //     std::cout << "(" << temp[j].layer << ", " << temp[j].x << ", " << temp[j].y << ") ";
        // }
        // std::cout << std::endl << std::endl;
    }

    return results;
}