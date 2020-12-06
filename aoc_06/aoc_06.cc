// Copyright 2020 HalfsInner
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "HolidayBag.hh"

template <typename Container>
auto crack_v1(Container &&customDeclarations) {
    auto t = HolidayBag::SportTimer("p1", "us", 1);

    return std::accumulate(cbegin(customDeclarations), cend(customDeclarations), 0,
        [](auto &&sum, auto &&groupDeclarations) {
            std::unordered_set<char> declarations;
            for (auto &&person : groupDeclarations) {
                declarations.insert(cbegin(person), cend(person));
            }

            return sum + declarations.size();
        });
}

template <typename Container>
auto crack_v2(Container &&customDeclarations) {
    auto t = HolidayBag::SportTimer("p2", "us", 1);

    return std::accumulate(cbegin(customDeclarations), cend(customDeclarations), 0,
        [](auto &&sum, auto &&groupDeclarations) {
            std::unordered_map<char, size_t> declarations;
            for (auto &&person : groupDeclarations) {
                for (auto &&c : person) {
                    ++declarations[c];
                }
            }
            const auto result = std::accumulate(cbegin(declarations), cend(declarations), 0,
                [groupSize = groupDeclarations.size()](
                    auto &&sum, auto &&el) { return (el.second == groupSize) ? sum + 1 : sum; });
            return sum + result;
        });
}

auto parsePuzzle(std::fstream &&istream) {
    auto t = HolidayBag::SportTimer("Parsing", "us");
    std::vector<std::vector<std::string>> customDeclarations {};

    for (std::string s {}; std::getline(istream, s, '\n');) {
        if (!s.empty() && s.back() == '\r') {
            s.pop_back();
        }
        if (s.empty() || customDeclarations.empty()) {
            customDeclarations.push_back({});
            continue;
        }
        auto &groupDeclarations = customDeclarations.back();
        groupDeclarations.emplace_back(s);
    }

    return customDeclarations;
}

int main(int argc, char *argv[]) {
    auto t = HolidayBag::SportTimer("Total", "us");

    std::cout << std::string(79, '-') << '\n';
    std::cout << "AoC2020_06 v1 & v2\n";
    if (argc != 2) {
        std::cerr << "Not enough (" << argc << ") arguments\n";
        return -1;
    }

    auto puzzle = std::fstream(argv[1], std::ios_base::in);
    if (!puzzle.is_open()) {
        std::cerr << "File '" << argv[1] << "' cannot be open\n";
        return -2;
    }

    auto customDeclarations = parsePuzzle(std::move(puzzle));
    std::cout << "Answer v1: " << crack_v1(customDeclarations) << '\n';
    std::cout << "Answer v2: " << crack_v2(customDeclarations) << '\n';

    t.stop();
    std::cout << t.getInterSummaryBag().unknit();
}
