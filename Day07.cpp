#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day07.exe inputfilename" << std::endl;
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
    std::vector<int> pos;
    while (true)
    {
        size_t p = s.find(',');
        if (p != std::string::npos)
        {
            pos.push_back(std::stoi(s.substr(0, p)));
            s = s.substr(p + 1);
        }
        else
        {
            pos.push_back(std::stoi(s));
            break;
        }
    }

    int min = std::numeric_limits<int>::max(), max = std::numeric_limits<int>::min();
    for (int p : pos)
    {
        min = std::min(min, p);
        max = std::max(max, p);
    }

    int part1 = std::numeric_limits<int>::max();
    int part2 = part1;
    for (int i = min+1; i < max; ++i)
    {
        int tmp1 = 0, tmp2 = 0;
        for (int p : pos)
        {
            int dist = std::abs(i - p);
            tmp1 += dist;
            tmp2 += dist * (dist + 1) / 2;
        }
        part1 = std::min(part1, tmp1);
        part2 = std::min(part2, tmp2);
    }

    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}