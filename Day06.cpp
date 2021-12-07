#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <numeric>

using u64 = unsigned long long;
#define ALLc(x) (x).cbegin(),(x).cend()

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day06.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::array<u64, 9> fishCount{0, 0, 0, 0, 0, 0, 0};
    
    std::string s;
    in >> s;
    for (size_t index = 0; index < s.size(); index += 2)
        ++fishCount[s[index] - '0'];
    
    u64 part1;
    for (size_t d = 0; d < 256; ++d)
    {
        if (d == 80)
            part1 = std::accumulate(ALLc(fishCount), 0ull);
        u64 c = fishCount[7];
        fishCount[7] = fishCount[8];
        fishCount[8] = fishCount[d % 7];
        fishCount[d % 7] += c;
    }

    u64 part2 = std::accumulate(ALLc(fishCount), 0ull);
    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}
