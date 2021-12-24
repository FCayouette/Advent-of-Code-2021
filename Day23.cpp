#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <unordered_set>
#include <queue>

int Evaluate(const std::string& startState, const std::string& target)
{
    constexpr std::array<int, 7> validTopPos{ 0, 1, 3, 5, 7, 9, 10 };
    constexpr std::array<int, 4> roomIndex = { 2, 4, 6, 8 }, typeCosts = { 1, 10, 100, 1000 };
    using CostState = std::pair<int, std::string>;
    std::unordered_set<std::string> evaluatedStates;
    std::priority_queue<CostState, std::vector<CostState>, std::greater<CostState>> toEvaluate;

    auto MoveToRoom = [&roomIndex, &typeCosts](const std::string& state, int pos) -> CostState
    {
        char type = state[pos];
        int target = roomIndex[type - 'A'], work = pos, cost = 0;
        do
        {
            work += (pos > target) ? -1 : 1;
            ++cost;
            if (state[work] != '.')
                return { 0, "" };
        } while (work != target);
        for (int i = work; i < state.size(); i += 11)
            if (state[i] != '.' && state[i] != type)
                return { 0, "" };

        while (work + 11 < state.size() && state[work + 11] == '.')
        {
            work += 11;
            ++cost;
        }
        std::string newState = state;
        std::swap(newState[pos], newState[work]);
        return { typeCosts[type - 'A'] * cost, newState };
    };

    auto MoveFromRoomTo = [&typeCosts, &roomIndex](const std::string& state, int from, int to) -> CostState
    {
        char type = state[from];
        int cost = 0, work = from;
        
        while (work > 10)
        {
            work -= 11;
            ++cost;
            if (state[work] != '.')
                return { 0, "" };
        }
        bool canMove = false;
        char target = 'A' + std::distance(roomIndex.cbegin(), std::find(roomIndex.cbegin(), roomIndex.cend(), work));
        for (int i = work + 11; i < state.size(); i += 11)
            if (state[i] != '.' && state[i] != target)
                canMove = true;
        if (!canMove)
            return { 0, "" };

        while (work != to)
        {
            work += (work < to ? 1 : -1);
            ++cost;
            if (state[work] != '.')
                return { 0, "" };
        }
        std::string newState = state;
        std::swap(newState[from], newState[to]);
        return { typeCosts[type - 'A'] * cost, newState };
    };

    toEvaluate.push({ 0, startState });

    while (!toEvaluate.empty())
    {
        auto [baseCost, baseState] = toEvaluate.top();
        toEvaluate.pop();
        if (baseState == target)
            return baseCost;

        if (auto [_, s] = evaluatedStates.insert(baseState);
            !s)
            continue;

        for (int pos : validTopPos)
            if (baseState[pos] != '.')
                if (auto [cost, newState] = MoveToRoom(baseState, pos);
                    cost > 0)
                    toEvaluate.push({ cost + baseCost, newState });
        int work = 0;
        while ((work += 11) < baseState.size())
            for (int delta : roomIndex)
                if (baseState[work + delta] != '.')
                    for (int targetPos : validTopPos)
                        if (auto [cost, newState] = MoveFromRoomTo(baseState, work + delta, targetPos);
                            cost > 0)
                            toEvaluate.push({ cost + baseCost, newState });
    }
    return -1;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Day23.exe inputfilename" << std::endl;
        return -1;
    }

    std::ifstream in(argv[1], std::ios::in);
    if (!in)
    {
        std::cout << "Could not open " << argv[1] << std::endl;
        return -1;
    }

    std::string s, target = "...........##A#B#C#D## #A#B#C#D#";

    in >> s >> s;
    std::string startState = s.substr(1, s.size() - 2);
    in >> s;
    startState += s.substr(1, s.size() - 2);
    in >> s;
    startState += ' ' + s;        

    std::cout << "Part 1: " << Evaluate(startState, target);

    std::string newStartState = startState.substr(0, 22) + " #D#C#B#A#  #D#B#A#C# " + startState.substr(22);
    target += "  #A#B#C#D#  #A#B#C#D#";
    std::cout << "\nPart 2: " << Evaluate(newStartState, target) << std::endl;
    return 0;
}