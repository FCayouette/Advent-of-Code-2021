#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>

#define ALLc(x) (x).cbegin(),(x).cend()
#define ALL(x) (x).begin(),(x).end()

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

    std::array<char, 256> buffer;
    std::vector<std::pair<std::string, std::string>> inOuts;
    
    while (in.getline(&buffer[0], 256))
    {
        std::string line = &buffer[0];
        size_t pos = line.find('|');
        inOuts.emplace_back(line.substr(0, pos - 1), line.substr(pos + 2));
    }

    int part1 = 0, part2 = 0;
    auto CountPart1 = [&part1](size_t p)
    {
        if (p >= 2 && p <= 4 || p == 7)
            ++part1;
    };

    for (const auto& p : inOuts)
    {
        std::string s = p.second;
        std::string key = p.first;
        
        std::array<std::string, 10> sortedKeys;
        std::vector<std::string> split;

        auto Process = [&sortedKeys, &split](std::string s)
        {
            std::sort(ALL(s));
            size_t l = s.length();
            if (l == 2)
                sortedKeys[1] = s;
            else if (l == 3)
                sortedKeys[7] = s;
            else if (l == 4)
                sortedKeys[4] = s;
            else if (l == 7)
                sortedKeys[8] = s;
            else
                split.push_back(s);
        };

        while (true)
        {
            size_t p = key.find(' ');
            if (p != std::string::npos)
            {
                Process(key.substr(0, p));
                key = key.substr(p + 1);
            }
            else
            {
                Process(key);
                break;
            }
        }

        char topRight, bottomRight;
        for (auto s : split)
            if (s.size() == 6)
            {
                if (s.find(sortedKeys[1][0]) == std::string::npos)
                {
                    sortedKeys[6] = s;
                    topRight = sortedKeys[1][0];
                    bottomRight = sortedKeys[1][1];
                }
                else if (s.find(sortedKeys[1][1]) == std::string::npos)
                {
                    sortedKeys[6] = s;
                    topRight = sortedKeys[1][1];
                    bottomRight = sortedKeys[1][0];
                }
            }

        for (const auto& s : split)
            if (s.size() == 5)
            {
                if (s.find(bottomRight) != std::string::npos)
                    sortedKeys[s.find(topRight) != std::string::npos ? 3 : 5] = s;
                else sortedKeys[2] = s;
            }

        for (auto s : split)
            if (s.size() == 6 && s != sortedKeys[6])
            {
                bool nine = true;
                for (char c : s)
                    if (c != topRight && sortedKeys[5].find(c) == std::string::npos)
                    {
                        nine = false;
                        break;
                    }
                sortedKeys[nine ? 9 : 0] = s;
            }

        int output = 0;
        auto PushNum = [&output, &sortedKeys](std::string s)
        {
            std::sort(ALL(s));
            output = output * 10 + (int)(std::find(ALLc(sortedKeys), s) - sortedKeys.cbegin());
        };

        while (true)
        {
            size_t p = s.find(' ');
            if (p != std::string::npos)
            {
                CountPart1(p);
                PushNum(s.substr(0, p));
                s = s.substr(p + 1);
            }
            else
            {
                CountPart1(s.size());
                PushNum(s);
                break;
            }
        }
        part2 += output;
    }
    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;

    return 0;
}