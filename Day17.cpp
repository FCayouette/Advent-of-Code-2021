#include <iostream>
#include <fstream>
#include <string>

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
    int minX, minY, maxX, maxY, mx = 0, part2 = 0;
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
        int px = 0, py = 0;
        while (py >= minY && px <= maxX)
        {
            if (px >= minX && py <= maxY) 
                return true;
            px += vx;
            py += vy--;
            vx = std::max(0, vx - 1);
        }
        return false;
    };

    while (mx * (mx + 1) / 2 < minX)
        ++mx;
    for (int x = mx; x <= maxX; ++x)
        for (int y = minY, mY = -minY; y < mY; ++y)
            if (Simulate(x, y))
                ++part2;
    
    std::cout << "Part 1: " << minY*(minY+1)/2 << "\nPart 2: " << part2 << std::endl;
    return 0;
}