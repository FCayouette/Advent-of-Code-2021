#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>

#define ALLc(x) (x).cbegin(), (x).cend()

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day13.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    int part1 = 0, part2 = 0;
    std::string s;
    std::vector<std::pair<int, int>> points;
    while (in >> s)
    {
        if (s[0] != 'f')
        {
            int p = s.find(',');
            int x = std::stoi(s.substr(0, p));
            int y = std::stoi(s.substr(p + 1));
            points.emplace_back(x, y);
        }
        else
        {
            in >> s;
            in >> s;
            
            int val = std::stoi(s.substr(2));
            std::set<std::pair<int, int>> uniquePoints;
            
            for (auto p : points)
            {
                if (s[0] == 'x')
                    uniquePoints.insert(p.first < val ? p : std::make_pair(2*val - p.first, p.second));
                else
                    uniquePoints.insert(p.second < val ? p : std::make_pair(p.first, 2*val - p.second));
            }
            
            points.clear();
            for (auto p : uniquePoints)
                points.push_back(p);

            if (!part1)
                part1 = uniquePoints.size();
        }
    }

    int minX = 1000000, minY = 1000000, maxX = 0, maxY = 0;
    for (auto p : points)
    {
        minX = std::min(minX, p.first);
        minY = std::min(minY, p.second);
        maxX = std::max(maxX, p.first);
        maxY = std::max(maxY, p.second);
    }
    std::cout << "Part 1: " << part1 << "\nPart2: " << std::endl;

    for (int i = 0; i <= maxY; ++i)
    {
        for (int j = 0; j <= maxX; ++j)
            std::cout << (std::find(ALLc(points), std::make_pair(j, i)) != points.cend() ? (char)219 : ' ');
        std::cout << std::endl;
    }
    return 0;
}