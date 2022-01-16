#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <list>
#include <map>

#define ALL(x) (x).begin(), (x).end()
#define ALLc(x) (x).cbegin(), (x).cend()
using Vec3 = std::array<int, 3>;
using ScannerPoints = std::vector<Vec3>;
using Permut = std::array<Vec3, 3>;
using DistanceMap = std::map<int, std::pair<int, int>>;

Vec3 Transform(const Permut& p, const Vec3& v)
{
    Vec3 result;
    for (int i = 0; i < 3; ++i)
        result[i] = std::inner_product(ALLc(p[i]), v.cbegin(), 0);
    return result;
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

    std::list<int> scannersToProcess;
    std::vector<int> solved{ 0 };
    for (int i = 1; i < points.size(); ++i)
        scannersToProcess.push_back(i);

    ScannerPoints positions;
    positions.push_back({ 0,0,0 });
    int maxManhattan = 0;

    auto CrossProduct = [](const Vec3& l, const Vec3& r) { return Vec3{ l[1] * r[2] - r[1] * l[2], -l[0] * r[2] + r[0] * l[2], l[0] * r[1] - r[0] * l[1] }; };
    auto Negate = [](Vec3& v) { for (int i = 0; i < 3; ++i) v[i] = -v[i]; };
    auto Add = [](const Vec3& l, const Vec3& r) { return Vec3{ l[0] + r[0], l[1] + r[1], l[2] + r[2] }; };

    while (!scannersToProcess.empty())
        for (auto iter = scannersToProcess.begin(); iter != scannersToProcess.end();)
        {
            bool found = false;
            for (int sIndex : solved)
            {
                auto test1 = distances[*iter].begin(), test2 = distances[sIndex].begin();
                int count = 0;
                while (test1 != distances[*iter].end() && test2 != distances[sIndex].end())
                {
                    if (test1->first < test2->first)
                        ++test1;
                    else if (test2->first < test1->first)
                        ++test2;
                    else if (++count >= 55)
                    {
                        const Vec3& s1 = points[sIndex][test2->second.first], &s2 = points[sIndex][test2->second.second];
                        const Vec3& p1 = points[*iter][test1->second.first], &p2 = points[*iter][test1->second.second];
                        Vec3 ds{ std::abs(s1[0] - s2[0]), std::abs(s1[1] - s2[1]), std::abs(s1[2] - s2[2]) };
                        Vec3 dp{ std::abs(p1[0] - p2[0]), std::abs(p1[1] - p2[1]), std::abs(p1[2] - p2[2]) };
                        Permut transform;
                        for (int i = 0; i < 2; ++i)
                            for (int j = 0; j < 3; ++j)
                                transform[i][j] = ds[i] == dp[j];
                        
                        for (int t = 0; t < 4; ++t)
                        {
                            if (t == 2)
                                Negate(transform[0]);
                            else if (t % 2)
                                Negate(transform[1]);    
                            transform[2] = CrossProduct(transform[0], transform[1]);

                            std::array<Vec3, 2> np = { Transform(transform, p1), Transform(transform, p2) };
                            for (int i = 0; i < 2; ++i)
                                if (Vec3 scannerPos = { s1[0] - np[i][0], s1[1] - np[i][1], s1[2] - np[i][2] }; 
                                    std::find(ALLc(points[sIndex]), Add(scannerPos, np[(i+1)%2])) != points[sIndex].cend())
                                {
                                    ScannerPoints transformed;
                                    transformed.reserve(points[*iter].size());
                                    for (const Vec3& p : points[*iter])
                                        transformed.emplace_back(Add(Transform(transform, p), scannerPos));
                                            
                                    std::swap(points[*iter], transformed);
                                    for (const Vec3& s : positions)
                                        maxManhattan = std::max(maxManhattan, std::inner_product(ALLc(s), scannerPos.cbegin(), 0, std::plus<>(), [](int a, int b) {return std::abs(a - b); }));

                                    positions.push_back(scannerPos);
                                    found = true;
                                    goto hasfound;
                                }
                        }
                    }
                    else
                    {
                        ++test1;
                        ++test2;
                    }
                }
            }
            hasfound:
            if (found)
            {
                solved.push_back(*iter);
                iter = scannersToProcess.erase(iter);
            }
            else ++iter;
        }

    readPoints.clear();
    for (const ScannerPoints& sp : points)
        readPoints.insert(readPoints.end(), ALLc(sp));

    std::sort(ALL(readPoints));
    readPoints.erase(std::unique(ALL(readPoints)), readPoints.end());
    std::cout << "Part 1: " << readPoints.size() << "\nPart 2: " << maxManhattan << std::endl;
    return 0;
}
