#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <set>
#include "TemplatedUtilities.h"

#define ALL(x) (x).begin(), (x).end()
#define ALLc(x) (x).cbegin(), (x).cend()
using i64 = long long;
using Vec3 = std::array<int, 3>;
using ScannerPoints = std::vector<Vec3>;
using Permut = std::array<Vec3, 3>;

Vec3 CrossProduct(const Vec3& l, const Vec3& r)
{
    Vec3 result;
    result[0] = l[1] * r[2] - r[1] * l[2];
    result[1] = -l[0] * r[2] + r[0] * l[2];
    result[2] = l[0] * r[1] - r[0] * l[1];
    return result;
}

Permut Generate(const Vec3& facing, const Vec3& up)
{
    Permut p;
    p[0] = facing;
    p[1] = up;
    p[2] = CrossProduct(facing, up);
    return p;
}

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

    std::vector<ScannerPoints> rawPoints;
    ScannerPoints solvedPoints, readPoints;
    int count = 0;

    std::string s;
    while (in >> s)
    {
        if (s == "---") 
        {
            in >> s;
            in >> s;
            in >> s;
            if (count++)
            {
                if (solvedPoints.empty())
                    std::swap(solvedPoints, readPoints);
                else
                    rawPoints.push_back(std::move(readPoints));
            }
            readPoints.clear();
            continue;
        }
        auto p = s.find(',');
        int x = std::stoi(s.substr(0, p));
        s = s.substr(p + 1);
        p = s.find(',');
        int y = std::stoi(s.substr(0, p));
        int z = std::stoi(s.substr(p + 1));
        readPoints.push_back({ x, y, z });
    }
    rawPoints.push_back(std::move(readPoints));

    std::vector<Permut> permutations;
    std::array<Vec3, 6> dirs = { Vec3{1,0, 0}, Vec3{-1, 0, 0}, Vec3{0, 1, 0}, Vec3{0,-1,0}, Vec3{0,0,1}, Vec3{0,0,-1} };
    
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            if (i / 2 != j / 2)
                permutations.push_back(Generate(dirs[i], dirs[j]));

    std::set<int> scannersToProcess;
    for (int i = 0; i < rawPoints.size(); ++i)
        scannersToProcess.insert(i);

    std::sort(ALL(solvedPoints));

    std::vector<Vec3> positions;
    positions.push_back({ 0,0,0 });
    int maxManhattan = 0;

    while (!scannersToProcess.empty())
        for (auto iter = scannersToProcess.begin(); iter != scannersToProcess.end(); ++iter)
        {
            bool found = false;

            for (const Permut& perm : permutations)
            {
                ScannerPoints points = rawPoints[*iter];
                for (Vec3& v : points)
                {
                    Vec3 tmp;
                    for (int i = 0; i < 3; ++i)
                        tmp[i] = std::inner_product(ALLc(perm[i]), v.cbegin(), 0);
                    std::swap(v, tmp);
                }
            
                for (const Vec3& tentative : points)
                {
                    for (const Vec3& solved : solvedPoints)
                    {
                        Vec3 scannerPos{ solved[0]-tentative[0], solved[1]-tentative[1], solved[2]-tentative[2]};
                        
                        int count = 0;
                        for (const Vec3& v : points)
                            if (std::binary_search(ALLc(solvedPoints), Vec3{ scannerPos[0] + v[0], scannerPos[1] + v[1], scannerPos[2] + v[2] }) && ++count == 12)
                            {
                                found = true;
                                break;
                            };
                        if (found)
                        {
                            for (const Vec3& v : points)
                                solvedPoints.push_back(Vec3{ scannerPos[0] + v[0], scannerPos[1] + v[1], scannerPos[2] + v[2] });
                            
                            std::sort(ALL(solvedPoints));
                            solvedPoints.erase(std::unique(ALL(solvedPoints)), solvedPoints.end());

                            for (const Vec3& v : positions)
                                maxManhattan = std::max(maxManhattan, std::abs(v[0] - scannerPos[0]) + std::abs(v[1] - scannerPos[1]) + std::abs(v[2] - scannerPos[2]));
                            
                            positions.push_back(scannerPos);
                            break;
                        }
                    }
                    if (found) break;
                }
                if (found) break;
            }

            if (found)
            {
                scannersToProcess.erase(iter);
                break;
            }
        }

    std::cout << "Part 1: " << solvedPoints.size() << "\nPart 2: " << maxManhattan << std::endl;
    return 0;
}
