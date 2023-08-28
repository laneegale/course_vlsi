#include <iostream>
#include <filesystem>

#include "include/readfile.h"
#include "include/router.h"

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

void write_output(Vec2D<Point> results, std::string output_fp)
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
    outputFile << results.size() << std::endl;
    // outputFile << std::fixed << std::setprecision(8);
    for (int i = 0; i < results.size(); i++)
    {
        outputFile << i + 1 << std::endl;
        if (results[i].size() > 0)
        {
            // for each vector, print layer+1, x, y in reverse order
            for (int j = results[i].size() - 1; j >= 0; j--)
            {
                outputFile << results[i][j].layer + 1 << " " << results[i][j].x << " " << results[i][j].y << std::endl;
            }
        }
        outputFile << 0 << std::endl;
    }
    outputFile.close();
}

int main()
{
    std::string output_dir = "routing_results/";
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
    std::ifstream inputFile;

    inputFile.open("parameters");
    if (!inputFile)
    {
        std::cerr << "ERROR FROM main.cpp:: Unable to open file parameters";
        exit(1);
    }
    getline(inputFile, fp);
    inputFile.close();

    fp = strip(fp);
    output_fp = output_dir + strip(fp.substr(fp.find_last_of("/") + 1));

    Raw_data raw_data = read_data(fp);

    Vec2D<Point> results = maze_router(raw_data);

    write_output(results, output_fp);
    return 0;
}