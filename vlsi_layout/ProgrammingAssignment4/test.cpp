#include <iostream>

#include "include/readfile.h"

void test_operator()
{   
    std::cout << "Testing operator function" << std::endl << std::endl;

    std::cout << "Testing Point operator==" << std::endl;

    Point A{1, 2, 3};
    Point B{1, 2, 3};
    Point C{1, 2, 4};

    // cout A, B, C
    std::cout << "A: " << A.layer << " " << A.x << " " << A.y << std::endl;
    std::cout << "B: " << B.layer << " " << B.x << " " << B.y << std::endl;
    std::cout << "C: " << C.layer << " " << C.x << " " << C.y << std::endl;

    if (A == B) std::cout << "A == B" << std::endl;
    if (A == C) std::cout << "A == C" << std::endl;
    if (B == C) std::cout << "B == C" << std::endl;

    std::cout << "Finish testing operator==" << std::endl << std::endl;

    std::cout << "Testing Wave_unit operator > < == >= <= !=" << std::endl << std::endl;
    Wave_unit W1{A, 2, "E"};
    Wave_unit W2{A, 3, "W"};
    Wave_unit W3{B, 1, "E"}; 
    Wave_unit W4{B, 1, "W"};

    // cout the cost of each wave_unit
    std::cout << "W1 pathcost: " << W1.pathcost << std::endl;
    std::cout << "W2 pathcost: " << W2.pathcost << std::endl;
    std::cout << "W3 pathcost: " << W3.pathcost << std::endl;
    std::cout << "W4 pathcost: " << W4.pathcost << std::endl;

    // Testing operator> < >= <= == !=
    if (W1 > W2) std::cout << "W1 > W2" << std::endl;
    if (W1 < W2) std::cout << "W1 < W2" << std::endl;
    if (W1 >= W2) std::cout << "W1 >= W2" << std::endl;
    if (W1 <= W2) std::cout << "W1 <= W2" << std::endl;
    if (W1 == W2) std::cout << "W1 == W2" << std::endl;
    if (W1 != W2) std::cout << "W1 != W2" << std::endl;
    
    if (W1 > W3) std::cout << "W1 > W3" << std::endl;
    if (W1 < W3) std::cout << "W1 < W3" << std::endl;
    if (W1 >= W3) std::cout << "W1 >= W3" << std::endl;
    if (W1 <= W3) std::cout << "W1 <= W3" << std::endl;
    if (W1 == W3) std::cout << "W1 == W3" << std::endl;
    if (W1 != W3) std::cout << "W1 != W3" << std::endl;

    std::cout << std::endl;

    return;
}

void test_wave_front()
{   
    std::cout << "Testing min heap struecture" << std::endl;
    // Init a heap to store the wavefront, set it to be a min heap
    std::priority_queue<Wave_unit, Vec1D<Wave_unit>, std::greater<Wave_unit>> wavefront;

    // Init some waveunit
    Wave_unit A{Point{1, 2, 3}, 2, "E"};
    Wave_unit B{Point{1, 2, 3}, 3, "W"};
    Wave_unit C{Point{1, 2, 3}, 1, "E"};
    Wave_unit D{Point{1, 2, 3}, 7, "W"};
    Wave_unit E{Point{1, 2, 3}, 1, "E"};
    Wave_unit F{Point{1, 2, 3}, 10, "W"};

    // Push to heap
    wavefront.push(A);
    wavefront.push(B);
    wavefront.push(C);
    wavefront.push(D);
    wavefront.push(E);
    wavefront.push(F);

    // cout cost of wave_unit A to F in one row
    std::cout << "Cost of wave_unit A to F: ";
    std::cout << A.pathcost << " " << B.pathcost << " " << C.pathcost << " " << D.pathcost << " " << E.pathcost << " " << F.pathcost << std::endl;

    // cout the order of the heap
    std::cout << "Order of the heap: ";
    while (!wavefront.empty())
    {
        std::cout << wavefront.top().pathcost << " ";
        wavefront.pop();
    }
    std::cout << std::endl;

    return;
}

int main()
{
    srand((unsigned int) time(NULL));

    std::string benchmark_name = "benchmarks/bench1";    
    Raw_data data = read_data(benchmark_name);

    std::cout << "Read data from " << benchmark_name << std::endl;

    Grid_input grid = data.grid;
    Nl_input nl = data.nl;

    std::cout << "Cost grid width/Cols: " << grid.width << std::endl;
    std::cout << "Cost grid height/Rows: " << grid.height << std::endl;

    Costgrid cost_grid = data.grid.cost_grid;
    // Check if the Rows number equal to grid.height
    std::cout << "layers/cost_grid.size() = " << cost_grid.size() << std::endl;
    std::cout << "Cols/cost_grid[0][0].size() = " << cost_grid[0][0].size() << std::endl;
    std::cout << "Rows/cost_grid[0].size() = " << cost_grid[0].size() << std::endl;

    // count the total number of element in cost_grid[0]
    int total_element = 0;
    for (int i = 0; i < cost_grid[0].size(); i++)
    {
        total_element += cost_grid[0][i].size();
    }
    // Do the same for cost_grid[1]
    for (int i = 0; i < cost_grid[1].size(); i++)
    {
        total_element -= cost_grid[1][i].size();
    }
    if (total_element != 0)  std::cout << "Total elements not matched!!!!!!" << std::endl;
    else
    {
        std::cout << "Total elements matched!!!!!!" << std::endl << std::endl;
    }

    
    std::cout << "Wires number: " << nl.num_wires << std::endl;
    WireVec wires_info = data.nl.wires_info;
    std::cout << "Wires vector size: " << wires_info.size() << std::endl;

    // Seclect a random from 0 to wires_info.size()
    int random_wire_index = rand() % wires_info.size();
    std::cout << "Random wire check at wire " << random_wire_index+1 << ": ";
    // Print the wire info in one line
    std::cout << wires_info[random_wire_index].netID << " " << wires_info[random_wire_index].pin1.layer << " " << wires_info[random_wire_index].pin1.x << " " << wires_info[random_wire_index].pin1.y << " " << wires_info[random_wire_index].pin2.layer << " " << wires_info[random_wire_index].pin2.x << " " << wires_info[random_wire_index].pin2.y << std::endl << std::endl;


    test_operator();

    test_wave_front();

    return 0;
}