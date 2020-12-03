// Copyright 2020 HalfsInner
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "HolidayBag.hh"

struct Policy {
  int min = -1;
  int max = -1;
  char character;
  std::string password;
};

template <typename Container>
auto crack_v1(Container &&policies) {
  auto t = HolidayBag::SportTimer("p1", "us", 1);
  return std::accumulate(cbegin(policies), cend(policies), 0, [](auto &&sum, auto &&el) {
    auto res = std::count(cbegin(el.password), cend(el.password), el.character);
    return (res >= el.min && res <= el.max) ? sum + 1 : sum;
  });
}

template <typename Container>
auto crack_v2(Container &&policies) {
  auto t = HolidayBag::SportTimer("p2", "us", 1);
  return std::accumulate(cbegin(policies), cend(policies), 0, [](auto &&sum, auto &&el) {
    auto res = (el.password.at(el.min - 1) == el.character) +
               (el.password.at(el.max - 1) == el.character);
    return (res == 1) ? sum + 1 : sum;
  });
}

auto parsePuzzle(std::fstream &&istream) {
  auto t = HolidayBag::SportTimer("Parsing", "us");
  std::vector<Policy> input{};

  char buf[51];
  for (std::string s{}; std::getline(istream, s, '\n');) {
    Policy p;
    sscanf(s.c_str(), "%d-%d %c: %50s", &p.min, &p.max, &p.character, buf);
    p.password = std::string(buf);

    input.emplace_back(std::move(p));
  }

  return input;
}

int main(int argc, char *argv[]) {
  auto t = HolidayBag::SportTimer("Total", "us");

  std::cout << std::string(79, '-') << '\n';
  std::cout << "AoC2020_02 v1 & v2\n";
  if (argc != 2) {
    std::cerr << "Not enough (" << argc << ") arguments\n";
    return -1;
  }

  auto puzzle = std::fstream(argv[1], std::ios_base::in);
  if (!puzzle.is_open()) {
    std::cerr << "File '" << argv[1] << "' cannot be open\n";
    return -2;
  }

  auto policies = parsePuzzle(std::move(puzzle));
  std::cout << "Answer v1: " << crack_v1(policies) << '\n';
  std::cout << "Answer v2: " << crack_v2(policies) << '\n';

  t.stop();
  std::cout << t.getInterSummaryBag().unknit();
}
