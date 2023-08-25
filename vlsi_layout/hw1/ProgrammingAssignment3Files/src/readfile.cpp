#include "../include/readfile.h"

std::vector<std::string> splitString(const std::string &input, const char &delimiter)
{
    std::stringstream ss(input);
    std::string token;
    std::vector<std::string> tokens;

    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

std::string removeSpecialCharacters(const std::string &input)
{
    std::string result;

    for (char c : input)
    {
        if (std::isalnum(c) || c == ' ' || c == '.')
        {
            result += c;
        }
    }

    return result;
}

std::vector<std::string> clean_tokens(const std::vector<std::string> &tokens)
{
    std::vector<std::string> results{};
    for (std::string s : tokens)
    {
        if (s == "")
            continue;
        else
            results.push_back(s);
    }

    return results;
}

std::vector<std::string> process_line(const std::string &line)
{
    std::string line_wiht_num_only = removeSpecialCharacters(line);
    return clean_tokens(splitString(line_wiht_num_only, ' '));
}

Netlist read_data(const std::string &filename)
{
    std::ifstream inputFile;
    std::string line;
    std::vector<std::string> tokens;
    Gatemap net_to_gate;
    Gatemap gate_to_net;
    std::map<int, std::vector<double>> net_to_pad;

    int num_of_gates;
    int num_of_nets;

    inputFile.open(filename);
    if (inputFile.is_open())
    {
        // Read first two lines
        // Get the gate and net numbers
        getline(inputFile, line);
        tokens = process_line(line);
        num_of_gates = std::stoi(tokens[0]);
        num_of_nets = std::stoi(tokens[1]);

        // Process the next num_of_nets lines
        // Read net_to_gate and get a gate_to_net map
        // Format of each line: gate_id, num_of_connected_gates, connected_gate_id_1, connected_gate_id_2, ...
        for (int i = 0; i < num_of_gates; i++)
        {
            getline(inputFile, line);
            tokens = process_line(line);

            int gate_id = std::stoi(tokens[0]);
            int num_of_connected_nets = std::stoi(tokens[1]);

            for (int j = 0; j < num_of_connected_nets; j++)
            {
                int net_id = std::stoi(tokens[j + 2]);
                gate_to_net[gate_id].push_back(net_id);
                net_to_gate[net_id].push_back(gate_id);
            }
        }

        // Process the next 1 + num_of_nets of lines
        // Get the net_to_pad map
        // Format of each line: pad_id, connected_net_id, x_coord, y_coord
        getline(inputFile, line);
        tokens = process_line(line);
        int num_of_pads = std::stoi(tokens[0]);

        for (int i = 0; i < num_of_pads; i++)
        {
            getline(inputFile, line);
            tokens = process_line(line);

            double pad_id = std::stod(tokens[0]);
            double net_id = std::stod(tokens[1]);
            double x_coord = std::stod(tokens[2]);
            double y_coord = std::stod(tokens[3]);

            net_to_pad[net_id].push_back(pad_id);
            net_to_pad[net_id].push_back(x_coord);
            net_to_pad[net_id].push_back(y_coord);
        }

        inputFile.close();
    }
    else
    {
        std::cerr << "Unable to open the file." << std::endl;
        return Netlist();
    }

    // Derive the gate_to_gate and gate_to_pad map
    Gatemap gate_to_gate;
    Padmap gate_to_pad;

    // Combine the gate_to_net and net_to_gate to get gate_to_gate map
    for (int i = 0; i < num_of_gates; i++)
    {
        int current_gate_id = i + 1;
        std::vector<int> connected_nets = gate_to_net[current_gate_id];
        std::set<int> set_of_connected_gates;

        for (int current_net_id : connected_nets)
        {
            std::vector<int> connected_gates = net_to_gate[current_net_id];
            for (int connected_gate_id : connected_gates)
            {
                if (connected_gate_id != current_gate_id)
                {
                    set_of_connected_gates.insert(connected_gate_id);
                }
            }

            // Check if the gate is connected to a pad
            if (net_to_pad.find(current_net_id) != net_to_pad.end())
            {
                std::vector<double> cur_pad_info;
                cur_pad_info.push_back(net_to_pad[current_net_id][0]);
                cur_pad_info.push_back((double)current_net_id);
                cur_pad_info.push_back(net_to_pad[current_net_id][1]);
                cur_pad_info.push_back(net_to_pad[current_net_id][2]);
                gate_to_pad[current_gate_id].push_back(cur_pad_info);
            }
        }

        gate_to_gate[current_gate_id] = std::vector<int>(set_of_connected_gates.begin(), set_of_connected_gates.end());
    }

    // store the result to the struct DataSrc and return
    Netlist results;
    results.num_of_gates = num_of_gates;
    results.num_of_nets = num_of_nets;
    results.net_to_gates = net_to_gate;
    results.net_to_pads = net_to_pad;
    results.gate_to_gates = gate_to_gate;
    results.gate_to_pads = gate_to_pad;
    std::vector<int> gate_id = std::vector<int>(num_of_gates, 0);
    for (int i = 0; i < num_of_gates; i++)
    {
        gate_id[i] = i + 1;
    }
    results.gate_id = gate_id;

    return results;
}