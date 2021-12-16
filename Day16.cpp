#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <cassert>

#define ALLc(x) (x).cbegin(),(x).cend()
#define Bin2Int(x) std::stoll((x), nullptr, 2)
using i64 = long long;

i64 readValue(const std::string& s, int& from)
{
    bool go = true;
    std::string value = "";
    while (go)
    {
        go = s[from++] == '1';
        value += s.substr(from, 4);
        from += 4;
    }
    return Bin2Int(value);
}


i64 readPacket(const std::string& s, int& from, int& vSum)
{
    vSum += Bin2Int(s.substr(from, 3));
    int t = Bin2Int(s.substr(from + 3, 3));
    from += 6;
    if (t == 4)
        return readValue(s, from);
    int I = Bin2Int(s.substr(from++, 1));
    std::vector<i64> packetValues;

    if (!I)
    {
        int subLength = Bin2Int(s.substr(from, 15));
        from += 15;
        int target = from + subLength;
        while (from < target)
            packetValues.push_back(readPacket(s, from, vSum));
    }
    else
    {
        int subPackets = Bin2Int(s.substr(from, 11));
        from += 11;
        for (int i = 0; i < subPackets; ++i)
            packetValues.push_back(readPacket(s, from, vSum));
    }

    switch (t)
    {
    case 0: return std::accumulate(ALLc(packetValues), 0ll);
    case 1: return std::accumulate(ALLc(packetValues), 1ll, std::multiplies<>());
    case 2:
    {
        i64 m = packetValues.front();
        for (int i = 1; i<packetValues.size(); ++i)
            m = std::min(m, packetValues[i]);
        return m;
    }
    case 3:
    {
        i64 m = packetValues.front();
        for (int i = 1; i < packetValues.size(); ++i)
            m = std::max(m, packetValues[i]);
        return m;
    }
    case 5: return packetValues.front() > packetValues.back() ? 1ll : 0ll;
    case 6: return packetValues.front() < packetValues.back() ? 1ll : 0ll;
    default: return packetValues.front() == packetValues.back() ? 1ll : 0ll;
    }
}
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day16.exe inputfilename" << std::endl;
        return -1;
    }

    std::map<char, std::string> BITS;
    BITS['0'] = "0000";
    BITS['1'] = "0001";
    BITS['2'] = "0010";
    BITS['3'] = "0011";
    BITS['4'] = "0100";
    BITS['5'] = "0101";
    BITS['6'] = "0110";
    BITS['7'] = "0111";
    BITS['8'] = "1000";
    BITS['9'] = "1001";
    BITS['A'] = "1010";
    BITS['B'] = "1011";
    BITS['C'] = "1100";
    BITS['D'] = "1101";
    BITS['E'] = "1110";
    BITS['F'] = "1111";

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string input, bin="";
    in >> input;
    for (char c : input)
        bin += BITS[c];

    int index = 0, part1 = 0;
    i64 part2 = readPacket(bin, index, part1);

    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}
