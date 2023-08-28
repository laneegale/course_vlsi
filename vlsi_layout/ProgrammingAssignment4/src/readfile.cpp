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
        if (std::isalnum(c) || c == ' ' || c == '.' || c == '-')
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

Raw_data read_data(const std::string &benchmark_name)
{
    std::ifstream inputFile;
    std::string line;
    std::vector<std::string> tokens;

    std::string grid_fp = benchmark_name + ".grid";
    std::string nl_fp = benchmark_name + ".nl";

    Raw_data raw_data;

    int num_cols;
    int num_rows;
    int bend_penalty;
    int via_penalty;

    inputFile.open(grid_fp);
    if (inputFile.is_open())
    {
        // Read first line
        getline(inputFile, line);
        tokens = process_line(line);
        num_cols = std::stoi(tokens[0]);
        num_rows = std::stoi(tokens[1]);
        bend_penalty = std::stoi(tokens[2]);
        via_penalty = std::stoi(tokens[3]);

        raw_data.grid.width = num_cols;
        raw_data.grid.height = num_rows;

        // Process the next num_rows lines
        for (int ii = 0; ii < 2; ii++)
        {
            std::vector<std::vector<int>> cost_array;
            for (int i = 0; i < num_rows; i++)
            {
                getline(inputFile, line);
                tokens = process_line(line);
                std::vector<int> row;
                for (int j = 0; j < num_cols; j++)
                {
                    row.push_back(std::stoi(tokens[j]));
                }
                cost_array.push_back(row);
            }
            raw_data.grid.cost_grid.push_back(cost_array);
        }
    }
    else
    {
        std::cerr << "ERROR FROM readfile.cpp:: Unable to open file " << grid_fp;
        exit(1);
    }
    inputFile.close();

    inputFile.open(nl_fp);
    if (inputFile.is_open())
    {
        // Read first line
        getline(inputFile, line);
        tokens = process_line(line);
        raw_data.nl.num_wires = std::stoi(tokens[0]);

        // Process the next num_wires lines
        for (int i = 0; i < raw_data.nl.num_wires; i++)
        {
            getline(inputFile, line);
            tokens = process_line(line);
            std::vector<Point> wire_info;
            Point pin1{std::stoi(tokens[1])-1, std::stoi(tokens[2]), std::stoi(tokens[3])};
            Point pin2{std::stoi(tokens[4])-1, std::stoi(tokens[5]), std::stoi(tokens[6])};
            raw_data.nl.wires_info.push_back(Wire{i+1, pin1, pin2});
        }
    }
    else
    {
        std::cerr << "ERROR FROM readfile.cpp:: Unable to open file " << nl_fp;
        exit(1);
    }
    inputFile.close();

    return raw_data;
}