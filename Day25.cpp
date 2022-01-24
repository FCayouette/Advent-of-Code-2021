#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day25.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string s;
    std::vector<std::string> map;
    while (in >> s)
        map.emplace_back(std::move(s));

    int result = 0, moveCount;
    do
    {
        moveCount = 0;
        ++result;
        for (std::string& row : map)
        {
            char r0 = row[0]; 
            int i = 1;
            for (; i < row.size(); ++i)
                if (row[i-1] == '>' && row[i] == '.')
                {
                    std::swap(row[i-1], row[i]);
                    ++moveCount;
                    ++i;
                }
            if (i == row.size() && row[i - 1] == '>' && r0 == '.')
            {
                ++moveCount;
                std::swap(row[i - 1], row[0]);
            }
        }

        for (int i = 0; i < map.front().size(); ++i)
        {
            char m0 = map[0][i];
            int j = 1;
            for (; j < map.size(); ++j)
                if (map[j-1][i] == 'v' && map[j][i] == '.')
                {
                    std::swap(map[j-1][i], map[j][i]);
                    ++moveCount;
                    ++j;
                }
            if (j == map.size() && map.back()[i] == 'v' && m0 == '.')
            {
                std::swap(map.back()[i], map.front()[i]);
                ++moveCount;
            }
        }
    } while (moveCount);

    std::cout << "Result: " << result << std::endl;
    return 0;
}
