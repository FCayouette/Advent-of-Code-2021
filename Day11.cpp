#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <set>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day10.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::array<std::array<int, 10>, 10> map;
    std::string s;
    for (int i = 0; i < 10; ++i)
    {
        in >> s;
        for (int j = 0; j < 10; ++j)
            map[i][j] = s[j] - '0';
    }

    int part1 = 0, part2 = 0;
    
    auto flashAt = [&map](int x, int y)
    {
        for (int i = std::max(0, x - 1), maxI = std::min(9, x + 1); i <= maxI; ++i)
            for (int j = std::max(0, y - 1), maxJ = std::min(9, y + 1); j <= maxJ; ++j)
                if (i != x || j != y)
                    ++map[i][j];
    };

    for (int turn = 0; !part2 || turn < 100; ++turn)
    {
        for (auto& r : map)
            for (auto& v : r)
                ++v;

        std::set<std::pair<int, int>> flashed;
        bool hasFlashed;
        do
        {
            hasFlashed = false;
            for (int x = 0; x < 10; ++x)
                for (int y = 0; y < 10; ++y)
                    if (map[x][y] > 9 && flashed.insert(std::make_pair(x, y)).second)
                    {
                        if (turn < 100)
                            ++part1;
                        hasFlashed = true;
                        flashAt(x, y);
                    }
        } while (hasFlashed);

        if (!part2 && flashed.size() == 100)
            part2 = turn + 1;
        for (auto& r : map)
            for (auto& v : r)
                if (v > 9)
                    v = 0;
    }

    std::cout << "Part 1: " << part1 << "\n Part 2: " << part2 << std::endl;
    return 0;
}