#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>

using i64 = long long;
#define ALL(x) (x).begin(),(x).end()

struct Region {
    Region() = default;
    Region(int a, int b, int c, int d, int e, int f) : minX(a), maxX(b), minY(c), maxY(d), minZ(e), maxZ(f) {}
    i64 Volume() const { return (maxX + 1ll - minX) * (maxY + 1ll - minY) * (maxZ + 1ll - minZ); }
    constexpr bool Intersects(const Region& r)
    {
        if (maxX < r.minX || minX > r.maxX ||
            maxY < r.minY || minY > r.maxY ||
            maxZ < r.minZ || minZ > r.maxZ)
            return false;
        return true;
    }
    Region Intersection(const Region& r) const
    {
        std::array<int, 4> work{ minX, maxX, r.minX, r.maxX };
        std::sort(ALL(work));
        int x = work[1], X = work[2];
        work = { minY, maxY, r.minY, r.maxY };
        std::sort(ALL(work));
        int y = work[1], Y = work[2];
        work = { minZ, maxZ, r.minZ, r.maxZ };
        std::sort(ALL(work));
        return Region(x, X, y, Y, work[1], work[2]);
    }
    int minX, maxX, minY, maxY, minZ, maxZ;
};

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

    std::string s;
    i64 part1 = 0, part2 = 0;
    std::vector<Region> toAdd, toRemove;
    while (in >> s)
    {
        char c;
        int minX, maxX, minY, maxY, minZ, maxZ;
        in >> c >> c >> minX >> c >> c >> maxX >> c >> c >> c >> minY >> c >> c>> maxY;
        in >> c >> c >> c >> minZ >> c >> c >> maxZ;

        if (!part1 && (minX > 50 || maxX < -50 || minY >50 || maxY < -50 || minZ > 50 || maxZ < -50))
        {
            for (const Region& r : toAdd)
                part1 += r.Volume();
            for (const Region& r : toRemove)
                part1 -= r.Volume();
        }
        Region region(minX, maxX, minY, maxY, minZ, maxZ);
        std::vector<Region> work;
        if (s == "on")
        {
            for (const Region& r : toAdd)
                if (region.Intersects(r))
                    work.emplace_back(region.Intersection(r));
            toAdd.push_back(region);
            for (const Region& r : toRemove)
                if (region.Intersects(r))
                    toAdd.emplace_back(region.Intersection(r));
            toRemove.insert(toRemove.end(), ALL(work));
        }
        else
        {
            for (const Region& r : toRemove)
                if (region.Intersects(r))
                    work.push_back(region.Intersection(r));
            for (const Region& r : toAdd)
                if (region.Intersects(r))
                    toRemove.emplace_back(region.Intersection(r));
            toAdd.insert(toAdd.end(), ALL(work));
        }
    }

    for (const Region& r : toAdd)
        part2 += r.Volume(); 
    for (const Region& r : toRemove)
        part2 -= r.Volume();
    
    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}