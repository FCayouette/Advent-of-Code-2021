#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

using Connections = std::map<std::string, std::vector<std::string>>;

int Count(const Connections& c, std::string pos, std::vector<std::string>& path)
{
    if (pos == "end")
        return 1;
    if (char c = pos[0];
        c >= 'a' && c <= 'z' || c=='0')
        if (std::find(path.cbegin(), path.cend(), pos) != path.cend())
            return 0;
    path.push_back(pos);
    int count = 0;
    for (const std::string& s : c.find(pos)->second)
            count += Count(c, s, path);
    path.pop_back();

    return count;
}

int Count2(const Connections& c, std::string pos, std::vector<std::string>& path, bool twice)
{
    if (pos == "end")
        return 1;
 
    if (char c = pos[0];
        c >= 'a' && c <= 'z')
    {
        bool multi = std::count(path.cbegin(), path.cend(), pos) > 0;
        if (twice && multi)
            return 0;
        twice = twice || multi;
    }

    path.push_back(pos);
    int count = 0;
    for (const std::string& s : c.find(pos)->second)
        if (s[0] != '0')
            count += Count2(c, s, path, twice);
    path.pop_back();

    return count;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day12.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string s;
    Connections connections;
    while (in >> s)
    {
        auto i = s.find('-');
        std::string a = s.substr(0, i);
        std::string b = s.substr(i + 1);
        if (a == "start")
            a = "0";
        if (b == "start")
            b = "0";

        connections[a].push_back(b);
        connections[b].push_back(a);
    }

    std::vector<std::string> path;
    std::cout << "Part 1: " << Count(connections, "0", path) << "\nPart 2: " << Count2(connections, "0", path, false) << std::endl;
    return 0;
}