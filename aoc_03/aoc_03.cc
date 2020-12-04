// Copyright 2020 HalfsInner
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "HolidayBag.hh"

enum class ForestEntity : char { tree = '#', grass = '.' };

template <typename Container>
auto traverse(Container &&forestMap, const std::pair<size_t, size_t> &slope) {
  auto solutions = 0;
  for (size_t y = 0, x = 0; y < forestMap.size(); y += slope.second) {
    if (forestMap.at(y).at(x) == ForestEntity::tree) {
      ++solutions;
    }
    x += slope.first;
    // Modulo equivalent
    if (x >= forestMap.at(y).size()) {
      x -= forestMap.at(y).size();
    }
  }
  return solutions;
}

template <typename Container>
auto crack_v1(Container &&forestMap) {
  auto t = HolidayBag::SportTimer("p1", "us");
  return traverse(forestMap, {3, 1});
}

template <typename Container>
auto crack_v2(Container &&forestMap) {
  auto t = HolidayBag::SportTimer("p2", "us");
  const std::vector<std::pair<size_t, size_t>> slopes{{1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

  std::vector<size_t> answers;
  std::transform(cbegin(slopes), cend(slopes), std::back_inserter(answers),
                 [&forestMap](auto &&slope) { return traverse(forestMap, slope); });

  return std::accumulate(cbegin(answers), cend(answers), 1llu, std::multiplies<uint64_t>());
}

auto parseMap(std::fstream &&istream) {
  auto t = HolidayBag::SportTimer("Parsing", "us");
  std::vector<std::vector<ForestEntity>> forestMap{};

  for (std::string s{}; std::getline(istream, s, '\n');) {
    if (s.back() == '\r') {
      s.pop_back();
    }
    std::vector<ForestEntity> forestLine;
    std::transform(cbegin(s), cend(s), std::back_inserter(forestLine),
                   [](auto &&c) { return (c == '#') ? ForestEntity::tree : ForestEntity::grass; });

    forestMap.emplace_back(forestLine);
  }

  return forestMap;
}

int main(int argc, char *argv[]) {
  auto t = HolidayBag::SportTimer("Total", "us");

  std::cout << std::string(79, '-') << '\n';
  std::cout << "AoC2020_03 v1 & v2\n";
  if (argc != 2) {
    std::cerr << "Not enough (" << argc << ") arguments\n";
    return -1;
  }

  auto puzzle = std::fstream(argv[1], std::ios_base::in);
  if (!puzzle.is_open()) {
    std::cerr << "File '" << argv[1] << "' cannot be open\n";
    return -2;
  }

  auto forestMap = parseMap(std::move(puzzle));
  std::cout << "Answer v1: " << crack_v1(forestMap) << '\n';
  std::cout << "Answer v2: " << crack_v2(forestMap) << '\n';

  t.stop();
  std::cout << t.getInterSummaryBag().unknit();
}
