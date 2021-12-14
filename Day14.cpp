#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <array>
#include <map>

using u64 = unsigned long long;
#define ALLc(x) (x).cbegin(),(x).cend()
#define ALL(x) (x).begin(), (x).end()

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day14.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    size_t part1 = 0, part2 = 0;
    std::string s;
    in >> s;

    std::array<u64, 26> hist;
    hist.fill(0ull);
    ++hist[s[0] - 'A'];

    std::map<std::pair<char, char>, u64> counts, work;
    for (int i = 1; i < s.size(); ++i)
    {
        ++counts[{s[i - 1], s[i]}];
        ++hist[s[i] - 'A'];
    }

    std::map<std::pair<char, char>, char> reactions;
    while (in >> s)
    {
        std::string tmp;
        in >> tmp;
        char c;
        in >> c;
        reactions[{s[0], s[1]}] = c;
    }

    for (int i = 0; i < 40; ++i)
    {
        if (i == 10)
        {
            std::array<u64, 26> histogram = hist;
            std::sort(ALL(histogram), std::greater<>());
            std::cout << "Part 1: " << histogram[0] - *(std::prev(std::find(ALLc(histogram), 0ull))) << std::endl;
        }

        for (const auto& i : counts)
        {
            char c = reactions[i.first];
            u64 toAdd = i.second;
            hist[c - 'A'] += toAdd;
            work[{i.first.first, c}] += toAdd;
            work[{c, i.first.second}] += toAdd;
        }
       
        std::swap(work, counts);
        work.clear();
    }

    std::sort(ALL(hist), std::greater<>());
    std::cout << "Part 2: " << hist[0] - *(std::prev(std::find(ALLc(hist), 0))) << std::endl;
    
    return 0;
}