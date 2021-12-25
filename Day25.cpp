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
            std::string work = row;
            for (int i = 0; i < row.size(); ++i)

                if (row[i] == '>' && row[(i + 1) % row.size()] == '.')
                {
                    std::swap(work[i], work[(i + 1) % row.size()]);
                    ++moveCount;
                }
            row = work;
        }

        for (int i = 0; i < map.front().size(); ++i)
        {
            std::string work = "";
            for (const std::string& s : map)
                work += s[i];
            for (int j = 0; j < map.size(); ++j)
                if (map[j][i] == 'v' && map[(j + 1) % map.size()][i] == '.')
                {
                    std::swap(work[j], work[(j + 1) % map.size()]);
                    ++moveCount;
                }
            for (int j = 0; j < work.size(); ++j)
                map[j][i] = work[j];
        }
    } while (moveCount);
    
    std::cout << "Result: " << result << std::endl;
    return 0;
}
