#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day03.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    using Line = std::array<int, 4> ;
    std::vector<Line> diagonalLines, straightLines;

    char comma1, comma2;
    int x1, x2, y1, y2;
    std::string arrow;
    while (in >> x1 >> comma1 >> y1 >> arrow >> x2 >> comma2 >> y2)
    {
        Line l = { x1, y1, x2, y2 };
        if (x1 == x2 || y1 == y2)
            straightLines.push_back(l);
        else 
            diagonalLines.push_back(l);
    }

    std::map<std::pair<int, int>, int> counts;

    auto AddCount = [&counts](int x, int y)
    {
        std::pair<int, int> p(x, y);
        if (auto iter = counts.find(p);
            iter == counts.cend())
        {
            counts[p] = 1;
            return 0;
        }
        else
            return (int)(++iter->second == 2);
    };

    int part1 = 0;
    for (const Line& l : straightLines)
    {
        if (l[1] == l[3])
            for (int i = std::min(l[0], l[2]), maxI = std::max(l[0], l[2]);  i <= maxI; ++i)
                part1 += AddCount(i, l[1]);
        else
            for (int i = std::min(l[1], l[3]), maxI = std::max(l[1], l[3]); i <= maxI; ++i)
                part1 += AddCount(l[0], i);
    }

    int part2 = part1;

    for (const Line& l : diagonalLines)
    {
        int minX, maxX, startY;
        bool risingY;
        if (l[0] < l[2])
        {
            minX = l[0];
            maxX = l[2];
            startY = l[1];
            risingY = l[1] < l[3];
        }
        else
        {
            minX = l[2];
            maxX = l[0];
            startY = l[3];
            risingY = l[3] < l[1];
        }

        for (int x = minX, y = startY; x <= maxX; ++x, y += risingY ? 1 : -1)
            part2 += AddCount(x, y);
    }
    
    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}