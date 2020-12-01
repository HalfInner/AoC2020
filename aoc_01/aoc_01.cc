// Copyright 2020 HalfsInner
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>

template <typename T>
auto crack_v1(T report) {
  int result = -1;
  for (auto &&val : report) {
    constexpr int xmassYear = 2020;
    const int supplement = xmassYear - val;
    auto it = report.find(supplement);
    if (it != cend(report)) {
      return val * *it;
    }
  }
  return result;
}

template <typename T>
auto crack_v2(T report) {
  int result = -1;
  for (auto it = cbegin(report); it != cend(report); ++it) {
    constexpr int xmassYear = 2020;
    const int supplement = xmassYear - *it;
    for (auto jt = it; jt != cend(report); ++jt) {
      const int nestedSupplement = supplement - *jt;
      auto itFind = report.find(nestedSupplement);
      if (itFind != cend(report)) {
        return *it * *jt * *itFind;
      }
    }
  }
  return result;
}

auto parsePuzzle(std::fstream &&istream) {
  std::unordered_set<int> input{};
  for (std::string s{}; std::getline(istream, s, '\n');) {
    input.emplace(std::stoi(s));
  }

  return input;
}

int main(int argc, char *argv[]) {
  std::cout << std::string(79, '-') << '\n';
  std::cout << "AoC2020_01 v1 & v2\n";
  if (argc != 2) {
    std::cerr << "Not enough (" << argc << ") arguments\n";
    return -1;
  }

  auto puzzle = std::fstream(argv[1], std::ios_base::in);
  if (!puzzle.is_open()) {
    std::cerr << "File '" << argv[1] << "' cannot be open\n";
    return -2;
  }

  auto expense_report = parsePuzzle(std::move(puzzle));
  std::cout << "Answer v1: " << crack_v1(expense_report) << '\n';
  std::cout << "Answer v2: " << crack_v2(expense_report) << '\n';
}
