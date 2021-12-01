#include <iostream>
#include <fstream>
#include <deque>
#include <numeric>

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

    int i, previous, prevWindow = -1, part1 = 0, part2 = 0;
    std::deque<int> slidingWindow;
    in >> previous;
    slidingWindow.push_back(previous);
    while (in >> i)
    {
        if (previous < i)
            ++part1;
        previous = i;
        
        slidingWindow.push_back(i);
        if (slidingWindow.size() > 3)
            slidingWindow.pop_front();
        if (slidingWindow.size() == 3)
        {
            int curr = std::accumulate(slidingWindow.cbegin(), slidingWindow.cend(), 0);
            if (prevWindow != -1 && curr > prevWindow)
                ++part2;
            prevWindow = curr;
        }
    }
    
    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}
