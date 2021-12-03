#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

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

    std::vector<size_t> counts;
    std::string s;
    std::vector<std::string> o2, co2;
    while (in >> s)
    {
        if (counts.size() == 0)
            counts.resize(s.size());
        for (int i = 0; i < s.size(); ++i)
            if (s[i] == '1') counts[i]++;
        o2.push_back(s);
        co2.push_back(s);
    }

    size_t part1a = 0, part1b = 0;
    for (size_t c : counts)
    {
        part1a *= 2;
        part1b *= 2;
        if (c >= o2.size() / 2)
            ++part1a;
        else
            ++part1b;
    }
    std::cout << "Part 1: " << part1a * part1b << std::endl;

    for (size_t i = 0; i < s.size(); ++i)
    {
        auto OneCounter = [i](const std::string& s) { return s[i] == '1'; };
        if (o2.size() > 1)
        {
            size_t count = std::count_if(ALLc(o2), OneCounter);
            o2.erase(std::remove_if(ALL(o2), [i, keep = count >= o2.size() - count ? '1' : '0'](const std::string& s) { return s[i] != keep; }), o2.end());
        }
        if (co2.size() > 1)
        {
            size_t count = std::count_if(ALLc(co2), OneCounter);
            co2.erase(std::remove_if(ALL(co2), [i, keep = count >= co2.size() - count ? '0' : '1'](const std::string& s) { return s[i] != keep; }), co2.end());
        }
    }
    std::cout << "Part 2: " << std::stoi(o2[0], nullptr, 2) * std::stoi(co2[0], nullptr, 2) << std::endl;

    return 0;
}