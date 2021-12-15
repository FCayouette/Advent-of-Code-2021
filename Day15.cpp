#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>

using Matrix = std::vector<std::vector<int>>;

struct AstarVal
{
    size_t pos, cost, estimatedCost, total;
    constexpr AstarVal(size_t p = 0, size_t c = 0, size_t ec = (size_t)-1) : pos(p), cost(c), estimatedCost(ec), total(c + ec) {}
    AstarVal& operator=(const AstarVal&) = default;
    constexpr bool operator>(const AstarVal& r) const { return total > r.total || (total == r.total && r.pos > pos); }
};

int FindPath(Matrix& matrix)
{
    int p00 = matrix[0][0];
    const size_t rows = matrix.size(), cols = matrix.front().size(), target = cols * rows - 1;
    constexpr int computed = -1;

    std::priority_queue<AstarVal, std::vector<AstarVal>, std::greater<AstarVal>> queue;
    queue.emplace(0, matrix[0][0], (cols + rows - 2));
    while (!queue.empty())
    {
        AstarVal val = queue.top();
        queue.pop();
        if (val.pos == target)
            return (int)val.cost - p00;
        size_t y = val.pos / cols, x = val.pos % cols;
        if (matrix[y][x] == computed)
            continue;
        matrix[y][x] = computed;

        if (x > 0 && matrix[y][x - 1] != computed)
            queue.emplace(val.pos - 1, matrix[y][x - 1] + val.cost, val.estimatedCost + 1);
        if (y > 0 && matrix[y - 1][x] != computed)
            queue.emplace(val.pos - cols, matrix[y - 1][x] + val.cost, val.estimatedCost + 1);
        if (x + 1 < cols && matrix[y][x + 1] != computed)
            queue.emplace(val.pos + 1, matrix[y][x + 1] + val.cost, val.estimatedCost - 1);
        if (y + 1 < rows && matrix[y + 1][x] != computed)
            queue.emplace(val.pos + cols, matrix[y + 1][x] + val.cost, val.estimatedCost - 1);
    }
    return 0;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day15.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    Matrix matrix, expandedMatrix, fullMatrix;
    std::string s;
    while (in >> s)
    {
        std::vector<int> r;
        for (char c : s)
            r.push_back(c - '0');
        matrix.push_back(r);
        std::vector<int> extended = r;
        for (int i = 1; i < 5; ++i)
            for (int j : r)
                extended.push_back(j + i <= 9 ? j + i : j + i - 9);
        expandedMatrix.push_back(extended);
    }

    fullMatrix = expandedMatrix;
    for (int i = 1; i < 5; ++i)
        for (const auto& line : expandedMatrix)
        {
            std::vector<int> f;
            for (int j : line)
                f.push_back(j + i <= 9 ? j + i : j + i - 9);
            fullMatrix.push_back(f);
        }
    
    std::cout << "Part 1: " << FindPath(matrix) << "\nPart 2: " << FindPath(fullMatrix) << std::endl;
    return 0;
}