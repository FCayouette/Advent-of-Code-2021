#include <iostream>
#include <fstream>
#include <deque>
#include <numeric>
#include <array>
#include <vector>
#include <algorithm>

template <typename T, size_t SIZE>
struct SlidingWindow
{
    void Insert(T val)
    {
        if (index >= SIZE and data[index % SIZE] < val) ++count;
        data[index++ % SIZE] = val;
    }
    std::array<T, SIZE> data = {};
    size_t index = 0, count = 0;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day01.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    SlidingWindow<int, 1> Part1;
    SlidingWindow<int, 3> Part2;

    int i;
    while (in >> i)
    {
        Part1.Insert(i);
        Part2.Insert(i);
    }
    
    std::cout << "Part 1: " << Part1.count << "\nPart 2: " << Part2.count << std::endl;
    return 0;
}
