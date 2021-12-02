#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day02.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string dir;
    int val, depth = 0, move = 0, depth2 = 0;

    while (in >> dir >> val)
        switch (dir[0])
        {
        case 'u': depth -= val; break;
        case 'd': depth += val; break;
        default: // 'f'
            move += val;
            depth2 += val * depth;
        }
    
    std::cout << "Part 1: " << move * depth << "\nPart 2: " << move * depth2 << std::endl;

    return 0;
}