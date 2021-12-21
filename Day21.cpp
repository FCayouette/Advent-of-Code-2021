#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <algorithm>
#include <map>

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

    int pos1, pos2, score1 = 0, score2 = 0, dice = 1, rolls = 0, p1, p2;

    in >> s >> pos1 >> s >> s >> pos1;
    in >> s >> pos2 >> s >> s >> pos2;

    p1 = pos1;
    p2 = pos2;

    auto Roll3 = [&dice, &rolls]()
    {
        int result = 0;
        for (int i = 0; i < 3; ++i)
        {
            result += dice++;
            if (dice > 100)
                dice = 1;
        }
        rolls += 3;
        return result;
    };

    bool player1 = true;
    while (score1 < 1000 && score2 < 1000)
    {
        int delta = Roll3();
        if (player1)
        {
            pos1 = (pos1 + delta) % 10;
            score1 += (!pos1 ? 10 : pos1);
        }
        else
        {
            pos2 = (pos2 + delta) % 10;
            score2 += (!pos2 ? 10 : pos2);
        }
        player1 = !player1;
    }

    std::cout << "Part 1: " << std::min(score1, score2) * rolls;
    
    using State = std::pair<std::pair<int, int>, std::pair<int, int>>;
    
    std::array<std::pair<int, i64>, 7> deltas = { std::make_pair(3,1ll), {4,3ll}, {5,6ll}, {6,7ll}, {7,6ll}, {8,3ll}, {9,1ll} };
    
    i64 wins1 = 0, wins2 = 0;

    std::map<State, i64> counts, work;
    player1 = true;
    counts[{ {0, 0}, { p1,p2 }}] = 1ll;
    while (!counts.empty())
    {
        if (player1)
            for (const auto& d : deltas)
                for (const auto& s : counts)
                {
                    State newState = s.first;
                    newState.second.first = (d.first + newState.second.first) % 10;
                    newState.first.first += (!newState.second.first ? 10 : newState.second.first);
                    if (newState.first.first >= 21)
                        wins1 += d.second * s.second;
                    else
                        work[newState] += s.second * d.second;
                }
        else
            for (const auto& d : deltas)
                for (const auto& s : counts)
                {
                    State newState = s.first;
                    newState.second.second = (d.first + newState.second.second) % 10;
                    newState.first.second += (!newState.second.second ? 10 : newState.second.second);
                    if (newState.first.second >= 21)
                        wins2 += d.second * s.second;
                    else
                        work[newState] += s.second * d.second;
                }
        std::swap(work, counts);
        work.clear();
        player1 = !player1;
    }

    std::cout << "\nPart 2: " << std::max(wins1, wins2) << std::endl;
    return 0;
}
