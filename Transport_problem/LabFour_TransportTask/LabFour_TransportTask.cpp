#include <iostream>
#include <vector>
#include <limits>

void printTable(const std::vector<std::vector<int>>& table, const std::vector<int>& supplies, const std::vector<int>& demands) {
    std::cout << "B1\tB2\tB3\tSupplies\n";
    for (size_t i = 0; i < table.size(); ++i) {
        std::cout << "A" << i + 1 << "\t";
        for (size_t j = 0; j < table[i].size(); ++j) {
            std::cout << table[i][j] << "\t";
        }
        std::cout << supplies[i] << "\n";
    }

    std::cout << "Demands\t";
    for (int demand : demands) {
        std::cout << demand << "\t";
    }
    std::cout << "\n\n";
}

void northwestCornerMethod(std::vector<std::vector<int>>& matrix, std::vector<int>& supplies, std::vector<int>& demands) {
    int i = 0;
    int j = 0;

    while (i < matrix.size() && j < matrix[0].size()) {
        int quantity = std::min(supplies[i], demands[j]);
        matrix[i][j] = quantity;
        supplies[i] -= quantity;
        demands[j] -= quantity;

        if (supplies[i] == 0) i++;
        if (demands[j] == 0) j++;
    }
}

void modifiedDistributionMethod(std::vector<std::vector<int>>& matrix, std::vector<int>& supplies, std::vector<int>& demands) {
    std::vector<int> u(matrix.size(), std::numeric_limits<int>::max());
    std::vector<int> v(matrix[0].size(), std::numeric_limits<int>::max());

    // Initialize potentials
    u[0] = 0;

    while (true) {
        bool change = false;

        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = 0; j < matrix[i].size(); ++j) {
                if (matrix[i][j] > 0) {
                    if (u[i] != std::numeric_limits<int>::max() && v[j] == std::numeric_limits<int>::max()) {
                        v[j] = matrix[i][j] - u[i];
                        change = true;
                    }
                    else if (v[j] != std::numeric_limits<int>::max() && u[i] == std::numeric_limits<int>::max()) {
                        u[i] = matrix[i][j] - v[j];
                        change = true;
                    }
                }
            }
        }

        if (!change) {
            break;
        }
    }

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 0) {
                matrix[i][j] = u[i] + v[j];
            }
        }
    }
}

int calculateTotalCost(const std::vector<std::vector<int>>& matrix, const std::vector<std::vector<int>>& originalMatrix) {
    int totalCost = 0;
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            // Проверяем, отличаются ли значения в первоначальной и конечной матрицах
            if (matrix[i][j] != originalMatrix[i][j]) {
                totalCost += matrix[i][j] * originalMatrix[i][j];
            }
        }
    }
    return totalCost;
}

int main() {
    // Заполнение строк матрицы пользователем
    std::vector<std::vector<int>> originalMatrix;
    std::vector<int> supplies;
    std::vector<int> demands;

    int rows, cols;

    // Ввод размерности матрицы
    std::cout << "Enter the number of rows: ";
    std::cin >> rows;

    std::cout << "Enter the number of columns: ";
    std::cin >> cols;

    // Ввод исходной матрицы
    std::cout << "Enter the original matrix:\n";
    for (int i = 0; i < rows; ++i) {
        std::vector<int> row;
        for (int j = 0; j < cols; ++j) {
            int value;
            std::cout << "Enter the value for A" << i + 1 << "B" << j + 1 << ": ";
            std::cin >> value;
            row.push_back(value);
        }
        originalMatrix.push_back(row);
    }

    // Ввод запасов
    std::cout << "Enter the supply values:\n";
    for (int i = 0; i < rows; ++i) {
        int supply;
        std::cout << "Enter the supply value for A" << i + 1 << ": ";
        std::cin >> supply;
        supplies.push_back(supply);
    }

    // Ввод требований
    std::cout << "Enter the demand values:\n";
    for (int j = 0; j < cols; ++j) {
        int demand;
        std::cout << "Enter the demand value for B" << j + 1 << ": ";
        std::cin >> demand;
        demands.push_back(demand);
    }

    std::cout << "\nTransportation Problem:\n";
    printTable(originalMatrix, supplies, demands);

    // Создание копии исходной матрицы для хранения измененной матрицы
    std::vector<std::vector<int>> matrix = originalMatrix;

    northwestCornerMethod(matrix, supplies, demands);
    std::cout << "Initial Feasible Solution (Northwest Corner Method):\n";
    printTable(matrix, supplies, demands);

    modifiedDistributionMethod(matrix, supplies, demands);
    std::cout << "Optimal Solution (Modified Distribution Method):\n";
    printTable(matrix, supplies, demands);

    int totalCost = calculateTotalCost(matrix, originalMatrix);
    std::cout << "Minimum Cost: F(x) = " << totalCost << "\n";

    return 0;
}
