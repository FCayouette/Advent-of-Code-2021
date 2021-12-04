#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <set>

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

    std::string s;
    in >> s;
    
    std::vector<int> numbers;
    while (true)
    {
        size_t p = s.find(',');
        if (p != std::string::npos)
        {
            numbers.push_back(std::stoi(s.substr(0, p)));
            s = s.substr(p + 1);
        }
        else
        {
            numbers.push_back(std::stoi(s));
            break;
        }
    }

    using Grid = std::array < std::array<std::pair<int, bool>, 5>, 5>;
    std::vector<Grid> grids;
    while (in >> s)
    {
        Grid g;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
            {
                g[i][j].first = std::stoi(s);
                if (i != 4 || j != 4)
                    in >> s;
            }

        grids.push_back(g);
    }

    auto Bingo = [](const Grid& g)
    {
        bool hasBingo = false;
        for (int i = 0; i < 5 && !hasBingo; ++i)
            if (g[i][0].second && g[i][1].second && g[i][2].second && g[i][3].second && g[i][4].second ||
                g[0][i].second && g[1][i].second && g[2][i].second && g[3][i].second && g[4][i].second)
                hasBingo = true;

        if (!hasBingo)
            return 0;

        int sum = 0;
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                if (!g[i][j].second)
                    sum += g[i][j].first;

        return sum;
    };

    auto SetNum = [](Grid & g, int num)
    {
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j)
                if (g[i][j].first == num)
                {
                    g[i][j].second = true;
                    return;
                }
    };

    for (int i = 0; i < 4; ++i)
        for (Grid& g : grids)
            SetNum(g, numbers[i]);
    
    int part1 = 0, part2 = 0;
    std::set<int> filledGrids;
    for (int i = 4; i < numbers.size() && !part2; ++i)
        for (int j = 0; j < grids.size(); ++j)
            if (filledGrids.find(j) == filledGrids.cend())
            {
                SetNum(grids[j], numbers[i]);
                if (int bingo = Bingo(grids[j]))
                {
                    if (filledGrids.empty())
                        part1 = bingo * numbers[i];
                    filledGrids.insert(j);
                    if (filledGrids.size() == grids.size())
                        part2 = bingo * numbers[i];
                }
            }
    
    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}