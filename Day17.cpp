#include <iostream>
#include <fstream>
#include <string>
#include <numeric>

constexpr bool Within(int min, int val, int max)
{
    return min <= val && val <= max;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day16.exe inputfilename" << std::endl;
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
    in >> s;
    int minX, minY, maxX, maxY, maxPos = 0;
    in >> s;
    s = s.substr(2, s.size()-3);
    auto p = s.find('.');
    minX = std::stoi(s.substr(0, p));
    maxX = std::stoi(s.substr(p + 2));
    in >> s;
    s = s.substr(2);
    p = s.find('.');
    minY = std::stoi(s.substr(0, p));
    maxY = std::stoi(s.substr(p + 2));

    auto Simulate = [minX, maxX, minY, maxY](int vx, int vy)
    {
        int px = 0, py = 0, maxpy = 0;
        bool hit = false;
        while (!hit && py > minY && px <= maxX)
        {
            px += vx;
            py += vy--;
            maxpy = std::max(maxpy, py);
            if (Within(minX, px, maxX) && Within(minY, py, maxY))
                return maxpy;
            vx = std::max(0, vx - 1);
        }
        return -1;
    };

    int min = 0, maxVy = 0, part2 = 0;
    while (min * (min + 1) / 2 < minX)
        ++min;
    
    for (int i = 0; i < 100; ++i)
        if (int result = Simulate(min, i); 
            result > 0)
        {
            maxVy = i;
            maxPos = std::max(maxPos, result);
        }

    for (int x = min; x <= maxX; ++x)
        for (int y = minY; y <= maxVy; ++y)
            if (Simulate(x, y) >= 0)
                ++part2;
    
    std::cout << "Part 1: " << maxPos << "\nPart 2: " << part2 << std::endl;
    return 0;
}