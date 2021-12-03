#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>

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

    size_t values = 0;
    std::vector<int> counts;
    std::string s;
    std::list<std::string> oxygen, co2;
    while (in >> s)
    {
        if (counts.size() == 0)
        {
            counts.resize(s.size());
        }
        for (int i = 0; i < s.size(); ++i)
            if (s[i] == '1') counts[i]++;
        ++values;
        oxygen.push_back(s);
        co2.push_back(s);
    }

    int part1a = 0, part1b = 0;
    for (int c : counts)
    {
        part1a *= 2;
        part1b *= 2;
        if (c >= values / 2)
            ++part1a;
        else
            ++part1b;
    }
    std::cout << "Part 1: " << part1a*part1b << std::endl;

    auto Count = [](const std::list<std::string>& l, size_t index)
    {
        std::pair<int, int> result(0, 0);
        for (const std::string& s : l)
        {
            if (s[index] == '0')
                ++result.first;
            else
                ++result.second;
        }
        return result;
    };

    auto KeepOnly = [](std::list<std::string>& l, size_t index, char c)
    {
        for (auto iter = l.begin(); iter != l.end();)
        {
            if ((*iter)[index] != c)
                iter = l.erase(iter);
            else
                ++iter;
        }
    };

    for (int i = 0; i < s.size(); ++i)
    {
        if (oxygen.size() > 1)
        {
            auto count = Count(oxygen, i);
            char keep = count.second >= count.first ? '1' : '0';
            KeepOnly(oxygen, i, keep);
        }
        if (co2.size() > 1)
        {
            auto count = Count(co2, i);
            char keep = count.second >= count.first ? '0' : '1';
            KeepOnly(co2, i, keep);
        }
    }

    std::cout << "Part 2: " << std::stoi(oxygen.front(), nullptr, 2) * std::stoi(co2.front(), nullptr, 2) << std::endl;

    return 0;
}