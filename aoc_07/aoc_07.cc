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
auto crack_v1(Container &&luggageGroup) {
    auto t = HolidayBag::SportTimer("p1", "us", 1);

    int64_t sum = 0;
    for (auto &&[luggage, bags] : luggageGroup) {
        std::stack<std::string> bagsStack;
        bagsStack.push(luggage);
        auto isRunning = true;
        while (!bagsStack.empty() && isRunning) {
            auto bags = luggageGroup[bagsStack.top()];
            bagsStack.pop();

            for (auto &&[bag, _] : bags) {
                if (bag == "shiny gold") {
                    ++sum;
                    isRunning = false;
                    break;
                } else {
                    bagsStack.push(bag);
                }
            }
        }
    }

    return sum;
}

template <typename Container>
auto crack_v2(Container &&luggageGroup) {
    auto t = HolidayBag::SportTimer("p2", "us", 1);

    std::stack<std::pair<std::string, int64_t>> bagsStack;
    bagsStack.push({ "shiny gold", 1 });
    int64_t sum = 0;
    while (!bagsStack.empty()) {
        auto bags = luggageGroup[bagsStack.top().first];
        auto multiplication = bagsStack.top().second;
        bagsStack.pop();

        for (auto &&[bag, count] : bags) {
            bagsStack.push({ bag, count * multiplication });
            sum += multiplication * count;
        }
    }
    return sum;
}

auto parsePuzzle(std::fstream &&istream) {
    auto t = HolidayBag::SportTimer("Parsing", "us");
    std::unordered_map<std::string, std::unordered_map<std::string, int>> luggageGroup;

    std::regex luggageRe("(\\w+ \\w+)( bags contain ){1}(.+)");
    std::regex bagsRe("\\d+ \\w+ \\w+ bags?[,. ]{1,2}");
    for (std::string s {}; std::getline(istream, s, '\n');) {
        if (!s.empty() && s.back() == '\r') {
            s.pop_back();
        }
        for (std::sregex_iterator it(s.begin(), s.end(), luggageRe), it_end; it != it_end; ++it) {
            auto laggageName = (*it)[1];
            luggageGroup[laggageName] = {};
            std::string bags = (*it)[3];
            for (std::sregex_iterator jt(bags.begin(), bags.end(), bagsRe), jt_end; jt != jt_end;
                 ++jt) {
                std::string element = (*jt)[0];
                auto numberpos = element.find(' ', 1);
                int number = std::stoi(element.substr(0, numberpos));
                std::string name = element.substr(numberpos + 1, element.find(" bag") - 2);
                luggageGroup[laggageName][name] = number;
            }
        }
    }

    return luggageGroup;
}

int main(int argc, char *argv[]) {
    auto t = HolidayBag::SportTimer("Total", "us");

    std::cout << std::string(79, '-') << '\n';
    std::cout << "AoC2020_07 v1 & v2\n";
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
