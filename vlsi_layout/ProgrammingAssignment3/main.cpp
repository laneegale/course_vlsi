#include <iostream>
#include <string>
#include <filesystem>

#include "include/data.h"
#include "include/readfile.h"
#include "include/placement.h"

// Write a function to strip and remove special characters at the beginning and end from a string
std::string strip(std::string s)
{
    int start = 0;
    int end = s.length() - 1;
    while (start < end && !isalnum(s[start]))
    {
        start++;
    }
    while (end >= start && !isalnum(s[end]))
    {
        end--;
    }
    return s.substr(start, end - start + 1);
}

void write_output(std::vector<double> placement_x_coord, std::vector<double> placement_y_coord, std::string output_fp)
{
    // Write idx+1, place_x, place_y to a txt file, keep 6 decimal places
    std::ofstream outputFile;
    outputFile.open(output_fp);
    // If the file is not found, exit the program
    if (!outputFile)
    {
        std::cerr << "ERROR FROM main.cpp:: Unable to create export file " << output_fp;
        exit(1);
    }
    outputFile << std::fixed << std::setprecision(8);
    for (int i = 0; i < placement_x_coord.size(); i++)
    {
        outputFile << i + 1 << " " << placement_x_coord[i] << " " << placement_y_coord[i] << std::endl;
    }
    outputFile.close();
}

int main()
{
    std::string output_dir = "placement_results";
    // Create a directory for output if not exists
    if (!std::filesystem::exists(output_dir))
    {
        std::filesystem::create_directory(output_dir);
    }

    // Read txt data from the file parameters
    // The first line in the file is the fp
    // The second line is a int indicating the max iteration
    std::string fp;
    std::string output_fp;
    std::string max_iter_string;
    std::ifstream inputFile;

    inputFile.open("parameters");
    // If the file is not found, exit the program
    if (!inputFile)
    {
        std::cerr << "ERROR FROM main.cpp:: Unable to open file parameters";
        exit(1);
    }
    getline(inputFile, fp);
    getline(inputFile, max_iter_string);
    inputFile.close();

    fp = strip(fp);
    int max_iter = std::stoi(strip(max_iter_string));
    output_fp = "placement_results/" + strip(fp.substr(fp.find_last_of("/") + 1));

    // Read data from the file
    std::cout << "Reading file" << std::endl;
    Netlist net_info = read_data(fp);
    std::cout << "Finished reading file" << std::endl;

    // Perform placement
    std::cout << "Performing placement" << std::endl;
    placement(net_info, 0, max_iter, Point{0, 0}, Point{100, 100}, 0);
    std::cout << "Finished placement" << std::endl;

    // placement(Point{0, 0}, Point{100, 100}, 0, 6, 0);

    // Export result
    std::cout << "Exporting result" << std::endl;
    std::vector<double> place_x;
    std::vector<double> place_y;
    for (int i = 0; i < net_info.num_of_gates; i++)
    {
        place_x.push_back(net_info.gate_to_coord[i + 1].x);
        place_y.push_back(net_info.gate_to_coord[i + 1].y);
    }
    write_output(place_x, place_y, output_fp);
    std::cout << "Finished exporting result" << std::endl;

    return 0;
}