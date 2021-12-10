#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

#define ALL(x) (x).begin(),(x).end()
using u64 = unsigned long long;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day10.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string s;
    int part1 = 0;
    std::vector<u64> completions;
    while (in >> s)
    {
        std::vector<char> stack;
        bool corrupted = false;
        for (char c : s)
        {
            if (corrupted)
                break;
            if (c == '{' || c == '(' || c == '[' || c == '<')
                stack.push_back(c);
            else
            {
                if (c == '}' && stack.back() != '{')
                {
                    part1 += 1197;
                    corrupted = true;
                }
                else if (c == ')' && stack.back() != '(')
                {
                    part1 += 3;
                    corrupted = true;
                }
                else if (c == ']' && stack.back() != '[')
                {
                    part1 += 57;
                    corrupted = true;
                }
                else if (c == '>' && stack.back() != '<')
                {
                    part1 += 25137;
                    corrupted = true;
                }
                else
                    stack.pop_back();
            }
        }
        if (!corrupted)
        {
            u64 score = 0;
            while (!stack.empty())
            {
                score *= 5ull;
                char c = stack.back();
                stack.pop_back();
                switch (c)
                {
                case '(': score += 1ull; break;
                case '[': score += 2ull; break;
                case '{': score += 3ull; break;
                default:  score += 4ull;
                }
            }
            completions.push_back(score);
        }
    }

    std::sort(ALL(completions));
    std::cout << "Part 1: " << part1 << "\nPart 2: " << completions[completions.size()/2] << std::endl;
    return 0;
}