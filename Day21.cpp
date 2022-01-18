#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <unordered_map>

using i64 = long long;

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

    std::string s;
    int pos1, pos2, score1 = 0, score2 = 0, dice = 1, rolls = 0, p1, p2, s1, s2;

    in >> s >> pos1 >> s >> s >> pos1 >> s >> pos2 >> s >> s >> pos2;

    p1 = --pos1;
    p2 = --pos2;

    bool player1 = true;
    while (score1 < 1000 && score2 < 1000)
    {
        int delta = 3*dice + 3;
        dice = (dice + 3) % 10;
        ++rolls;
        if (player1)
        {
            pos1 = (pos1 + delta) % 10;
            score1 += pos1 + 1;
        }
        else
        {
            pos2 = (pos2 + delta) % 10;
            score2 += pos2 + 1;
        }
        player1 = !player1;
    }

    std::cout << "Part 1: " << std::min(score1, score2) * rolls * 3;
    
    using State = std::tuple<int, int, int, int>;
    struct HashState
    {
        size_t operator()(const State& s) const
        {
            int a, b, c, d;
            std::tie(a, b, c, d) = s;
            return (((a * 32ll) + b) * 32ll + c) * 32ll + d;
        }
    };
    
    std::array<std::pair<int, i64>, 7> deltas = { std::make_pair(3,1ll), {4,3}, {5,6}, {6,7}, {7,6}, {8,3}, {9,1} };
    
    i64 wins1 = 0, wins2 = 0;
    std::unordered_map<State, i64, HashState> counts, work;
    player1 = true;
    counts[std::make_tuple(0, 0, p1, p2)] = 1ll;
    while (!counts.empty())
    {
        for (const auto& [roll, occ] : deltas)
            for (const auto& [s, count] : counts)
            {
                std::tie(s1, s2, p1, p2) = s;
                if (player1)
                {
                    p1 = (roll + p1) % 10;
                    if ((s1 += p1 + 1) >= 21)
                    {
                        wins1 += occ * count;
                        continue;
                    }
                }
                else if (p2 = (roll + p2) % 10;
                    (s2 += p2 + 1) >= 21)
                {
                    wins2 += occ * count;
                    continue;
                }
                work[std::make_tuple(s1, s2, p1, p2)] += count * occ;
            }
        std::swap(work, counts);
        work.clear();
        player1 = !player1;
    }

    std::cout << "\nPart 2: " << std::max(wins1, wins2) << std::endl;
    return 0;
}
