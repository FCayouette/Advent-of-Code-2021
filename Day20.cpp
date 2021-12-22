#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day19.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string decode, s;
    in >> decode;
    bool blinking = (decode.front() == '#' && decode.back() == '.');
    
    struct PairHash { size_t operator()(const std::pair<int, int>& p) const { return (p.first + 50ll) * 256ll + p.second + 50ll; } };
    std::unordered_set<std::pair<int, int>, PairHash> litPoints, work;

    int maxY = -1, minX = 0, minY = 0, maxX;
    while (in >> s)
    {
        ++maxY;
        for (int x = 0; x < s.size(); ++x)
            if (s[x] == '#')
                litPoints.insert({ x, maxY });
        maxX = s.size()-1;
    }

    auto Within = [&minX, &maxX, &minY, &maxY](int x, int y) { return minX <= x && x <= maxX && minY <= y && y <= maxY; };

    for (int it = 0; it < 50; ++it)
    {
        if (it == 2)
            std::cout << "Part 1: " << litPoints.size();

        --minX; --minY; ++maxX; ++maxY;
        if (blinking && (it % 2 == 1))
        {
            for (int i = minX; i <= maxX; ++i)
            {
                litPoints.insert({ i, minY });
                litPoints.insert({ i, maxY });
            }
            for (int i = minY; i <= maxY; ++i)
            {
                litPoints.insert({ minX, i });
                litPoints.insert({ maxX, i });
            }
        }
        
        for (int x = minX; x <= maxX; ++x)
            for (int y = minY; y <= maxY; ++y)
            {
                int index = 0;
                for (int j = -1; j < 2; ++j)
                    for (int i = -1; i < 2; ++i)
                    {
                        index *= 2;
                        int nx = x + i, ny = y + j;
                        if (Within(nx, ny)) index += litPoints.find({ nx, ny }) != litPoints.cend();
                        else index += (blinking && (it % 2 == 1));
                    }
                if (decode[index] == '#')
                    work.insert({ x,y });
            }
        std::swap(litPoints, work);
        work.clear();
    }
    std::cout << "\nPart 2: " << litPoints.size() << std::endl;

    return 0;
}
