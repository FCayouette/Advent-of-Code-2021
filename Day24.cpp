#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct NumProcess
{
    NumProcess(std::ifstream& in)
    {
        std::string s;
        for (int i = 0; i < 13; ++i)
            in >> s;
        if ((push = s == "1"))
        {
            for (int i = 0; i < 33; ++i)
                in >> s;
            delta = std::stoi(s);
            for (int i = 0; i < 6; ++i)
                in >> s;
        }
        else
        {
            for (int i = 0; i < 3; ++i)
                in >> s;
            delta = std::stoi(s);
            for (int i = 0; i < 36; ++i)
                in >> s;
        }
    }

    int delta = 0;
    bool push = false;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day24.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string s, maxValue, minValue;
    std::vector<NumProcess> blocks;
    std::vector<std::pair<int, int>> stack;
    while (in >> s)
        blocks.emplace_back(in);

    for (int i = 0; i < 14; ++i)
    {
        if (blocks[i].push)
        {
            stack.push_back({ i, blocks[i].delta });
            maxValue += '0';
            minValue += '0';
        }
        else
        {
            auto [index, val] = stack.back();
            stack.pop_back();
            int diff = blocks[i].delta + val;
            int max_push = std::min(9, 9 - diff);
            maxValue[index] = '0' + max_push;
            maxValue += '0' + max_push + diff;

            int min_push = std::max(1, 1 - diff);
            minValue[index] = '0' + min_push;
            minValue += '0' + min_push + diff;
        }
    }
    
    std::cout << "Part 1: " << maxValue << "\nPart 2: " << minValue << std::endl;
    return 0;
}