#include <iostream>
#include <string>

#include "include/readfile.h"
#include "include/placement.h"

void test_solve_matrix_equation_1()
{
    std::vector<double> R{0, 0, 1, 1, 1, 2, 2};
    std::vector<double> C{0, 1, 0, 1, 2, 1, 2};
    std::vector<double> V{4.0, -1.0, -1.0, 4.0, -1.0, -1.0, 4.0};
    int n = 3;

    std::vector<double> ans = solve_matrix_equation(std::vector<std::vector<double>>{R, C, V}, std::vector<double>{3, 2, 3}, n);

    // Check if ans is {1, 1, 1}
    for (int i = 0; i < n; i++)
    {
        if (ans[i] - 1 > 1e-7)
        {
            std::cerr << "ERROR FROM main_test.cpp:: test_solve_matrix_equation_1" << std::endl;
            exit(1);
        }
    }

    return;
}

int main()
{
    test_solve_matrix_equation_1();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}