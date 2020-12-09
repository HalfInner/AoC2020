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
auto crack_v1(Container &&xmasData) {
    auto t = HolidayBag::SportTimer("p1", "us", 1);

    constexpr size_t preamble = 25;
    for (size_t idx = preamble; idx < xmasData.size(); ++idx) {
        std::unordered_set<int64_t> sums;
        for (size_t i = 0; i < preamble - 1; ++i) {
            for (size_t j = i + 1; j < preamble; ++j) {
                sums.insert(xmasData.at(idx - preamble + i) + xmasData.at(idx - preamble + j));
            }
        }
        int64_t curr = xmasData.at(idx);
        if (sums.count(curr) == 0) {
            return curr;
        }
    }

    return static_cast<int64_t>(-1);
}

template <typename Container>
auto crack_v2(Container &&xmasData, int64_t ans_v1) {
    auto t = HolidayBag::SportTimer("p2", "us", 1);

    size_t considerNumbers = 2;
    for (size_t considered = considerNumbers;; ++considered) {
        int64_t curr_sum = 0;
        std::deque<int64_t> vals;
        for (size_t j = 0; j < considered; ++j) {
            auto val = xmasData.at(j);
            curr_sum += val;
            vals.push_back(val);
        }

        for (size_t idx = considered; idx < xmasData.size(); ++idx) {
            if (curr_sum == ans_v1) {
                auto [min, max] = std::minmax_element(cbegin(vals), cend(vals));
                return *min + *max;
            }
            auto firstVal = vals.front();
            curr_sum -= firstVal;
            vals.pop_front();

            curr_sum += xmasData.at(idx);
            vals.push_back(xmasData.at(idx));
        }
    }

    return static_cast<int64_t>(-1);
}

auto parsePuzzle(std::fstream &&istream) {
    auto t = HolidayBag::SportTimer("Parsing", "us");

    std::vector<int64_t> xmasData;
    for (std::string s {}; std::getline(istream, s, '\n');) {
        if (!s.empty() && s.back() == '\r') {
            s.pop_back();
        }
        xmasData.emplace_back(std::stoll(s));
    }

    return xmasData;
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

    auto xmasData = parsePuzzle(std::move(puzzle));
    auto ans_v1 = crack_v1(xmasData);
    std::cout << "Answer v1: " << ans_v1 << '\n';
    std::cout << "Answer v2: " << crack_v2(xmasData, ans_v1) << '\n';

    t.stop();
    std::cout << t.getInterSummaryBag().unknit();
}
