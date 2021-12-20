#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#define ALLc(x) (x).cbegin(), (x).cend()

constexpr int PAD = 100;

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

    std::string decode, s;
    in >> decode;
    
    std::string largeString(300, '.');

    std::vector<std::string> work, largeImage(3*PAD, largeString);
    int row = PAD;
    while (in >> s)
    {
        for (int i = 0; i < s.size(); ++i)
            largeImage[row][i + PAD] = s[i];
        ++row;
    }
    work = largeImage;

    auto WithinImage = [](int x, int y) {
        return 0 <= x && x < 3*PAD && 0 <= y && y < 3*PAD;
    };

    auto Count = [&largeImage]()
    {
        int count = 0;
        for (const std::string& s : largeImage)
            count += std::count(ALLc(s), '#');
        return count;
    };

    for (int it = 0; it < 50; ++it)
    {
        if (it == 2)
            std::cout << "Part 1: " << Count() << std::endl;
        for (int i = 0; i < 3*PAD; ++i)
            for (int j = 0; j < 3*PAD; ++j)
            {
                int index = 0;
                for (int x = -1; x < 2; ++x)
                    for (int y = -1; y < 2; ++y)
                    {
                        index *= 2;
                        int a = i + x, b = j + y;
                        if (WithinImage(a, b))
                            index += (largeImage[a][b] == '#') ? 1 : 0;
                        else
                            index += it % 2;
                    }

                work[i][j] = decode[index];
            }
        std::swap(work, largeImage);
    }

    std::cout <<"Part 2: " << Count() << std::endl;
    return 0;
}
