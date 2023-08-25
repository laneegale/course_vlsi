#include "../include/placement.h"

// Write a template function to find element in a vector<T>, return the index of the element if found, return -1 if not found
template <typename T>
int find_ele(std::vector<T> vec, T element)
{
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i] == element)
        {
            return i;
        }
    }
    return -1;
}

std::tuple<SparseMatrix, std::vector<double>, std::vector<double>> generate_matrix(const Netlist &net_info, const Point &lower_left, const Point &upper_right)
{
    Gatemap gate_to_gates = net_info.gate_to_gates;
    Padmap gate_to_pads = net_info.gate_to_pads;
    std::vector<int> gate_id = net_info.gate_id;
    int num_of_gates = net_info.num_of_gates;
    std::map<int, Point> gate_to_coord = net_info.gate_to_coord;

    // init two zero value vector, for each vector value in the input gate_to_gates, find all the two item combination in this vector and push the first item to the first vecvtor and the second item to the second vector
    std::vector<double> x_pos;
    std::vector<double> y_pos;
    std::vector<double> val;

    std::map<int, std::vector<int>> gate_to_pseudo_pad;

    std::vector<double> b_x(num_of_gates, 0);
    std::vector<double> b_y(num_of_gates, 0);

    std::map<int, double> diagonal_val_counter;
    for (int i = 0; i < num_of_gates; i++)
    {
        diagonal_val_counter[i] = 0;
    }

    for (int i = 0; i < num_of_gates; i++)
    {
        int cur_gate_id = gate_id[i];
        std::vector<int> connected_gates = gate_to_gates[cur_gate_id];
        // for all two item combinations in this vector

        for (int j = 0; j < connected_gates.size(); j++)
        {
            int correspond_pos = find_ele<int>(gate_id, connected_gates[j]);
            if (correspond_pos == -1)
            {
                gate_to_pseudo_pad[cur_gate_id].push_back(connected_gates[j]);
                diagonal_val_counter[i] += 1;

                // set b_x[i] with  gate_to_coord[connected_gates[j]].x, limit the value to be within the region
                double the_x = gate_to_coord[connected_gates[j]].x;
                the_x = std::max(the_x, lower_left.x);
                the_x = std::min(the_x, upper_right.x);
                b_x[i] += the_x;

                // set b_y[i] with  gate_to_coord[connected_gates[j]].y, limit the value to be within the region
                double the_y = gate_to_coord[connected_gates[j]].y;
                the_y = std::max(the_y, lower_left.y);
                the_y = std::min(the_y, upper_right.y);
                b_y[i] += the_y;
            }
            x_pos.push_back(i);
            y_pos.push_back(correspond_pos);
            val.push_back(1.0);

            diagonal_val_counter[correspond_pos] += 1;
        }

        if (gate_to_pads.find(cur_gate_id) != gate_to_pads.end())
        {
            diagonal_val_counter[i] += gate_to_pads[cur_gate_id].size();
            for (int j = 0; j < gate_to_pads[cur_gate_id].size(); j++)
            {
                // set b_x[i] with  gate_to_coord[connected_gates[j]].x, limit the value to be within the region
                double the_x = gate_to_pads[cur_gate_id][j][2];
                the_x = std::max(the_x, lower_left.x);
                the_x = std::min(the_x, upper_right.x);
                b_x[i] += the_x;

                // set b_y[i] with  gate_to_coord[connected_gates[j]].y, limit the value to be within the region
                double the_y = gate_to_pads[cur_gate_id][j][3];
                the_y = std::max(the_y, lower_left.y);
                the_y = std::min(the_y, upper_right.y);
                b_y[i] += the_y;
            }
        }
    }

    for (int i = 0; i < val.size(); i++)
    {
        val[i] = -val[i];
    }

    for (int i = 0; i < num_of_gates; i++)
    {
        x_pos.push_back(i);
        y_pos.push_back(i);
        val.push_back(diagonal_val_counter[i]);
    }

    return std::make_tuple(SparseMatrix{x_pos, y_pos, val}, b_x, b_y);
}

// Solve matrix equation Ax = b
std::vector<double> solve_matrix_equation(const SparseMatrix &a_matrix, const std::vector<double> &b_vector, const int &matrix_size)
{
    coo_matrix A;
    vector<double> R = a_matrix[0];
    vector<double> C = a_matrix[1];
    vector<double> V = a_matrix[2];

    A.n = matrix_size;
    A.nnz = R.size();
    A.row.resize(A.nnz);
    A.col.resize(A.nnz);
    A.dat.resize(A.nnz);

    // Turn R, C into int type vector then valarray
    vector<int> R_int(R.begin(), R.end());
    vector<int> C_int(C.begin(), C.end());
    valarray<int> R_val(R_int.data(), R_int.size());
    valarray<int> C_val(C_int.data(), C_int.size());
    // Turn V into double type vector then valarray
    valarray<double> V_val(V.data(), V.size());

    // Turen b_vector into valarray
    valarray<double> b_val(b_vector.data(), b_vector.size());

    A.row = R_val;
    A.col = C_val;
    A.dat = V_val;

    // initialize as [1] * n
    valarray<double> x(1.0, A.n);
    // valarray<double> b(A.n);
    // A.matvec(x, b); // b = Ax
    A.solve(b_val, x);

    // turn x into vector
    vector<double> x_vec(begin(x), end(x));

    return x_vec;
}

// Calculate placement from net info
std::pair<std::vector<double>, std::vector<double>> calculate_placement(const Netlist &net_info, const Point &lower_left, const Point &upper_right)
{
    std::cout << "generate matrix" << std::endl;
    std::tuple<SparseMatrix, std::vector<double>, std::vector<double>> matrix_tuple = generate_matrix(net_info, lower_left, upper_right);
    SparseMatrix A = std::get<0>(matrix_tuple);
    std::vector<double> b_x = std::get<1>(matrix_tuple);
    std::vector<double> b_y = std::get<2>(matrix_tuple);

    std::cout << "solve x" << std::endl;
    std::vector<double> place_x = solve_matrix_equation(A, b_x, net_info.num_of_gates);

    std::cout << "solve y" << std::endl;
    std::vector<double> place_y = solve_matrix_equation(A, b_y, net_info.num_of_gates);

    return std::make_pair(place_x, place_y);
}

// Sort the netlist by x_coord, then by y_coord, lastly by gate_id
std::vector<int> sort_gates(const Netlist &net_info, const int &direction)
{
    std::map<int, Point> gate_to_coord = net_info.gate_to_coord;
    std::vector<int> gate_id = net_info.gate_id;

    // FIRST PART: FIND OUT WHCIH GATE IS IN THE FIRST HALF AND WHICH GATE IS IN THE SECOND HALF
    // Sort the gate_id, first by x_coord, then by y_coord lastly by gate_id
    std::vector<std::tuple<double, double, int>> sorted_gate_id;
    for (int i = 0; i < gate_id.size(); i++)
    {
        int cur_gate_id = gate_id[i];
        double x_pos = gate_to_coord[cur_gate_id].x;
        double y_pos = gate_to_coord[cur_gate_id].y;

        if (direction == 0)
        {
            sorted_gate_id.push_back(std::make_tuple(x_pos, y_pos, cur_gate_id));
        }
        else
        {
            sorted_gate_id.push_back(std::make_tuple(y_pos, x_pos, cur_gate_id));
        }
    }
    // Sort gate is in the format of (x_coord, y_coord, gate_id)
    std::sort(sorted_gate_id.begin(), sorted_gate_id.end());

    vector<int> result;
    for (int i = 0; i < sorted_gate_id.size(); i++)
    {
        result.push_back(std::get<2>(sorted_gate_id[i]));
    }

    return result;
}

// Split a netlist into half, need a direction. 0 for vertical, 1 for horizontal
std::pair<Netlist, Netlist> split_netlist(const Netlist &net_info, const int &direction)
{
    Gatemap gate_to_gates = net_info.gate_to_gates;
    Padmap gate_to_pads = net_info.gate_to_pads;

    Netlist first_half_net;
    Netlist second_half_net;

    std::vector<int> sorted_gate_id = sort_gates(net_info, direction);

    std::vector<int> first_gate_id;
    std::vector<int> second_gate_id;

    int first_num_of_gates = sorted_gate_id.size() / 2;
    int second_num_of_gates = sorted_gate_id.size() - first_num_of_gates;

    for (int i = 0; i < sorted_gate_id.size(); i++)
    {
        if (i < first_num_of_gates)
        {
            first_gate_id.push_back(sorted_gate_id[i]);
        }
        else
        {
            second_gate_id.push_back(sorted_gate_id[i]);
        }
    }

    first_half_net.num_of_gates = first_num_of_gates;
    first_half_net.gate_to_gates = gate_to_gates;
    first_half_net.gate_to_pads = gate_to_pads;
    first_half_net.gate_id = first_gate_id;
    first_half_net.gate_to_coord = net_info.gate_to_coord;

    second_half_net.num_of_gates = second_num_of_gates;
    second_half_net.gate_to_gates = gate_to_gates;
    second_half_net.gate_to_pads = gate_to_pads;
    second_half_net.gate_id = second_gate_id;
    second_half_net.gate_to_coord = net_info.gate_to_coord;

    return std::make_pair(first_half_net, second_half_net);
}

// Split a boundry into half
vector<Point> split_region(const Point &lower_left, const Point &upper_right, const int &direction)
{
    vector<Point> result;
    if (direction == 0)
    {
        double mid_x = (lower_left.x + upper_right.x) / 2;
        result.push_back(Point{lower_left.x, lower_left.y});
        result.push_back(Point{mid_x, upper_right.y});
        result.push_back(Point{mid_x, lower_left.y});
        result.push_back(Point{upper_right.x, upper_right.y});
    }
    else
    {
        double mid_y = (lower_left.y + upper_right.y) / 2;
        result.push_back(Point{lower_left.x, lower_left.y});
        result.push_back(Point{upper_right.x, mid_y});
        result.push_back(Point{lower_left.x, mid_y});
        result.push_back(Point{upper_right.x, upper_right.y});
    }
    return result;
}

// Generate the placement result and write into a file
void placement(Netlist &net_info, const int &cur_iter, const int &max_iter, const Point &bottom_left, const Point &upper_right, const int &direction)
{
    // Calculate placement coordinate first
    std::pair<std::vector<double>, std::vector<double>> placement_result = calculate_placement(net_info, bottom_left, upper_right);

    vector<double> x_coord = placement_result.first;
    vector<double> y_coord = placement_result.second;
    std::map<int, Point> gate_to_coord;

    for (int i = 0; i < net_info.num_of_gates; i++)
    {
        net_info.gate_to_coord[net_info.gate_id[i]] = Point{x_coord[i], y_coord[i]};
    }

    if (cur_iter == max_iter)
    {
        return;
    }

    std::pair<Netlist, Netlist> splited_pair = split_netlist(net_info, direction);
    Netlist First_half_net = splited_pair.first;
    Netlist Second_half_net = splited_pair.second;

    std::vector<Point> split_region_result = split_region(bottom_left, upper_right, direction);
    Point first_lower_left = split_region_result[0];
    Point first_upper_right = split_region_result[1];
    Point second_lower_left = split_region_result[2];
    Point second_upper_right = split_region_result[3];

    placement(First_half_net, cur_iter + 1, max_iter, first_lower_left, first_upper_right, 1 - direction);
    placement(Second_half_net, cur_iter + 1, max_iter, second_lower_left, second_upper_right, 1 - direction);

    // write the result coord back to net_info
    vector<int> first_net_gate_id = First_half_net.gate_id;
    for (int i = 0; i < first_net_gate_id.size(); i++)
    {
        int gate_id = first_net_gate_id[i];
        net_info.gate_to_coord[gate_id] = First_half_net.gate_to_coord[gate_id];
    }
    std::vector<int> second_half_id = Second_half_net.gate_id;
    for (int i = 0; i < second_half_id.size(); i++)
    {
        int gate_id = second_half_id[i];
        net_info.gate_to_coord[gate_id] = Second_half_net.gate_to_coord[gate_id];
    }

    return;
}
