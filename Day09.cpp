#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <set>

#define ALLc(x) (x).cbegin(),(x).cend()
#define ALL(x) (x).begin(),(x).end()
using u64 = unsigned long long;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day09.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::vector<std::string> heightMap;
    int maxI, maxJ;
    std::string s;
    while (in >> s)
    {
        maxJ = s.size()-1;
        heightMap.emplace_back(s);
    }
    maxI = heightMap.size()-1;
    int part1 = 0;

    auto RiskCheck = [&heightMap, maxI, maxJ](int i, int j)
    {
        bool lower = true;
        char val = heightMap[i][j];
        if (i > 0 && heightMap[i - 1][j] <= val) return 0;
        if (i < maxI && heightMap[i + 1][j] <= val) return 0;
        if (j > 0 && heightMap[i][j - 1] <= val) return 0;
        if (j < maxJ && heightMap[i][j + 1] <= val) return 0;
        return val - '0' + 1;
    };

    std::vector<u64> basinSizes;

    for (int i = 0; i <= maxI; ++i)
        for (int j = 0; j <= maxJ; ++j)
            if (int riskValue = RiskCheck(i, j))
            {
                part1 += riskValue;
                std::set<std::pair<int, int>> inBasin, toConsider, tmp;
                auto InsertNew = [&inBasin, &tmp](int i, int j)
                {
                    std::pair<int, int> p{ i, j };
                    if (inBasin.find(p) == inBasin.cend())
                        tmp.insert(p);
                };
                toConsider.insert({ i, j });
                while (!toConsider.empty())
                {
                    for (auto p : toConsider)
                    {
                        inBasin.insert(p);
                        if (p.first > 0 && heightMap[p.first - 1][p.second] != '9') InsertNew(p.first - 1, p.second);
                        if (p.first < maxI && heightMap[p.first + 1][p.second] != '9') InsertNew(p.first + 1, p.second);
                        if (p.second > 0 && heightMap[p.first][p.second - 1] != '9') InsertNew(p.first, p.second - 1);
                        if (p.second < maxJ && heightMap[p.first][p.second + 1] != '9') InsertNew(p.first, p.second + 1);
                    }
                    std::swap(toConsider, tmp);
                    tmp.clear();
                }

                basinSizes.push_back(inBasin.size());
            }

    std::sort(ALL(basinSizes), std::greater<>());

    std::cout << "Part 1: " << part1 << "\nPart 2: " << basinSizes[0] * basinSizes[1] * basinSizes[2] << std::endl;
    return 0;
}
