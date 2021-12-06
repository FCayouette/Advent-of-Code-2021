#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <numeric>

using u64 = unsigned long long;

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

    std::array<u64, 9> fishCount, tmp;
    for (int i = 0; i < 9; ++i)
        fishCount[i] = tmp[i] = 0;
    std::string s;
    in >> s;
    while (true)
    {
        size_t p = s.find(',');
        if (p != std::string::npos)
        {
            int index = std::stoi(s.substr(0, p));
            ++fishCount[index];
            s = s.substr(p + 1);
        }
        else
        {
            int index = std::stoi(s);
            ++fishCount[index];
            break;
        }
    }

    u64 part1, part2;
    for (int d = 0; d < 256; ++d)
    {
        if (d == 80)
            part1 = std::accumulate(fishCount.cbegin(), fishCount.cend(), 0ull);
        for (int i = 8; i > 0; --i)
            tmp[i - 1] = fishCount[i];
        tmp[6] += fishCount[0];
        tmp[8] = fishCount[0];
        std::swap(tmp, fishCount);
    }

    part2 = std::accumulate(fishCount.cbegin(), fishCount.cend(), 0ull);
    std::cout << "Part 1: " << part1 << "\nPart2 : " << part2 << std::endl;
    return 0;
}
