#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <list>

auto IsValue = [](int c) { return c >= 10; };

bool Reduce(std::list<int>& num)
{
    int depth = 0;
    auto iter = num.begin();
    while (iter != num.end())
    {
        if (*iter == 2)
            --depth;
        else if (*iter == 1)
        {
            if (++depth == 5)
            {
                //Explode
                auto work = iter;

                int left = *(++work) - 10;
                ++work;
                int right = *(++work) - 10;
                ++work;
                iter = num.erase(iter, ++work);
                iter = num.insert(iter, 10);
                work = iter;
                while (work != num.begin())
                {
                    --work;
                    if (IsValue(*work))
                    {
                        *work += left;
                        break;
                    }
                }
                work = ++iter;
                while (work != num.end())
                {
                    if (IsValue(*work))
                    {
                        *work += right;
                        break;
                    }
                    ++work;
                }
                return true;
            }
        }
        ++iter;
    }

    iter = num.begin();
    while (iter != num.end())
    {
        if (IsValue(*iter))
        {
            if (int val = *iter - 10;
                val >= 10)
            {
                iter = num.erase(iter);
                iter = num.insert(iter, 2);
                int left = val / 2;
                iter = num.insert(iter, 10 + (val - left));
                iter = num.insert(iter, 3);
                iter = num.insert(iter, 10 + left);
                iter = num.insert(iter, 1);
                return true;
            }
        }
        ++iter;
    }

    return false;
}

auto Convert = [](char c) {
    if (c == '[') return 1;
    if (c == ']') return 2;
    if (c == ',') return 3;
    return c - '0' + 10;
};

int Magnitude(std::list<int>::const_iterator& iter)
{
    int result = 0;
    if (*(++iter) == 1)
        result = 3 * Magnitude(iter);
    else 
        result = 3 * (*(iter++) - 10);
    ++iter;
    if (*iter == 1)
        result += 2 * Magnitude(iter);
    else 
        result += 2 * (*(iter++) - 10);
    ++iter;
    return result;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day16.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string s;
    std::list<int> snailNum;
    bool first = true;

    std::vector<std::string> numbers;

    while (in >> s)
    {
        numbers.push_back(s);
        if (!first)
        {
            snailNum.push_front(1);
            snailNum.push_back(3);
        }
        for (char c : s)
            snailNum.push_back(Convert(c));
        if (!first)
            snailNum.push_back(2);
        first = false;
        while (Reduce(snailNum)) {}
    }

    auto iter = snailNum.cbegin();
    int part1 = Magnitude(iter), part2 = 0;
    
    for (int i = 0; i < numbers.size(); ++i)
        for (int j = 0; j < numbers.size(); ++j)
            if (i != j)
            {
                snailNum.clear();
                snailNum.push_back(1);
                for (char c : numbers[i])
                    snailNum.push_back(Convert(c));
                snailNum.push_back(3);
                for (char c : numbers[j])
                    snailNum.push_back(Convert(c));
                snailNum.push_back(2);
                while (Reduce(snailNum)) {}
                iter = snailNum.cbegin();
                part2 = std::max(Magnitude(iter), part2);
            }

    std::cout << "Part 1: " << part1 << "\nPart 2: " << part2 << std::endl;
    return 0;
}
