#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <set>
#include <map>

#define ALL(x) (x).begin(), (x).end()
#define ALLc(x) (x).cbegin(), (x).cend()
using i64 = long long;
using Vec3 = std::array<int, 3>;
using ScannerPoints = std::vector<Vec3>;
using Permut = std::array<Vec3, 3>;
using DistanceMap = std::map<int, std::pair<int, int>>;

Vec3 CrossProduct(const Vec3& l, const Vec3& r)
{
    Vec3 result{ l[1] * r[2] - r[1] * l[2], -l[0] * r[2] + r[0] * l[2], l[0] * r[1] - r[0] * l[1] };
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

    std::vector<ScannerPoints> points;
    std::vector<DistanceMap> distances;
    ScannerPoints readPoints;
    DistanceMap workingDistances;
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
                points.push_back(std::move(readPoints));
                distances.push_back(std::move(workingDistances));
            }
            readPoints.clear();
            workingDistances.clear();
            continue;
        }
        auto p = s.find(',');
        int x = std::stoi(s.substr(0, p));
        s = s.substr(p + 1);
        p = s.find(',');
        int y = std::stoi(s.substr(0, p));
        int z = std::stoi(s.substr(p + 1));
        
        readPoints.push_back({ x, y, z });
        for (int i = 0; i < readPoints.size()-1; ++i)
        {
            int dist = std::inner_product(ALLc(readPoints.back()), readPoints[i].cbegin(), 0, std::plus<>(), [](int a, int b) {int d = a - b; return d * d; });
            workingDistances[dist] = { i, readPoints.size() - 1 };
        }
        
    }
    points.push_back(std::move(readPoints));
    distances.push_back(std::move(workingDistances));

    std::vector<Permut> permutations;
    std::array<Vec3, 6> dirs = { Vec3{1,0, 0}, Vec3{-1, 0, 0}, Vec3{0, 1, 0}, Vec3{0,-1,0}, Vec3{0,0,1}, Vec3{0,0,-1} };
    
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            if (i / 2 != j / 2)
                permutations.push_back(Generate(dirs[i], dirs[j]));

    std::set<int> scannersToProcess, solved{ 0 };
    for (int i = 1; i < points.size(); ++i)
        scannersToProcess.insert(i);

    ScannerPoints positions;
    positions.push_back({ 0,0,0 });
    int maxManhattan = 0;

    while (!scannersToProcess.empty())
        for (auto iter = scannersToProcess.begin(); iter != scannersToProcess.end(); ++iter)
        {
            bool found = false;
            for (auto iter2 = solved.cbegin(); !found && iter2 != solved.cend(); ++iter2)
            {
                auto test1 = distances[*iter].begin(), test2 = distances[*iter2].begin();
                int count = 0;
                while (!found && test1 != distances[*iter].end() && test2 != distances[*iter2].end())
                {
                    if (test1->first < test2->first)
                        ++test1;
                    else if (test2->first < test1->first)
                        ++test2;
                    else if (++count >= 55)
                        for (const auto& transform : permutations)
                        {
                            ScannerPoints transformed;
                            transformed.reserve(points[*iter].size());
                            for (const Vec3& p : points[*iter])
                            {
                                Vec3 tmp;
                                for (int i = 0; i < 3; ++i)
                                    tmp[i] = std::inner_product(ALLc(transform[i]), p.cbegin(), 0);
                                transformed.emplace_back(tmp);
                            }
                            Vec3 sPos = points[*iter2][test2->second.first];
                            for (int index : {test1->second.first, test1->second.second})
                            {
                                Vec3 pos = transformed[index];
                                Vec3 scannerPos{ sPos[0] - pos[0], sPos[1] - pos[1], sPos[2] - pos[2] };
                                int match = 0;
                                for (const Vec3& v : transformed)
                                    if (std::find(ALLc(points[*iter2]), Vec3{ scannerPos[0] + v[0], scannerPos[1] + v[1], scannerPos[2] + v[2] }) != points[*iter2].cend() && ++match == 12)
                                    {
                                        for (Vec3& t : transformed)
                                            for (int i = 0; i < 3; ++i)
                                                t[i] += scannerPos[i];

                                        points[*iter] = transformed;
                                        for (const Vec3& s : positions)
                                            maxManhattan = std::max(maxManhattan, std::abs(s[0] - scannerPos[0]) + std::abs(s[1] - scannerPos[1]) + std::abs(s[2] - scannerPos[2]));

                                        positions.push_back(scannerPos);
                                        found = true;
                                        break;
                                    }
                                if (found) break;
                            }
                            if (found) break;
                        }
                    else
                    {
                        ++test1;
                        ++test2;
                    }
                }
            }
            if (found)
            {
                solved.insert(*iter);
                scannersToProcess.erase(iter);
                break;
            }
        }

    readPoints.clear();
    for (const ScannerPoints& sp : points)
        readPoints.insert(readPoints.end(), ALLc(sp));

    std::sort(ALL(readPoints));
    readPoints.erase(std::unique(ALL(readPoints)), readPoints.end());
    
    std::cout << "Part 1: " << readPoints.size() << "\nPart 2: " << maxManhattan << std::endl;
    return 0;
}
