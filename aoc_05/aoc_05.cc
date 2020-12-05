// Copyright 2020 HalfsInner
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "HolidayBag.hh"

struct Seat {
  int row{-1};
  int column{-1};
  int64_t seatId{-1};

  Seat(int row, int column) : row(row), column(column) { seatId = row * 8 + column; }

  explicit Seat(std::string_view boarding) {
    std::bitset<10> seatencoded;
    seatencoded.reset();
    for (size_t i = 0; i < 10; ++i) {
      seatencoded[9 - i] = boarding.at(i) == 'B' || boarding.at(i) == 'R';
    }
    seatId = seatencoded.to_ulong();
    row = seatId >> 3;
    column = seatId & 0x7;
  }

  bool operator<(const Seat &s) const { return seatId < s.seatId; }
};

template <typename Container>
auto crack_v1(Container &&boardingPasses) {
  auto t = HolidayBag::SportTimer("p1", "us", 1);

  int64_t maxseatId = -1;
  for (auto &&boarding : boardingPasses) {
    const auto seat = Seat{boarding};
    maxseatId = std::max(maxseatId, seat.seatId);
  }
  return maxseatId;
}

template <typename Container>
auto crack_v2(Container &&boardingPasses) {
  auto t = HolidayBag::SportTimer("p1", "us", 1);

  std::set<Seat> seats{};
  std::transform(cbegin(boardingPasses), cend(boardingPasses), std::inserter(seats, cend(seats)),
                 [](auto &&boarding) { return Seat{boarding}; });

  Seat seatPrev{-1, -1};
  for (auto &&seat : seats) {
    if (seatPrev.column == -1) {
      seatPrev = seat;
      continue;
    }
    if (seat.seatId - seatPrev.seatId > 1) {
      return seat.seatId - 1;
    }
    seatPrev = seat;
  }

  return static_cast<int64_t>(-1);
}

auto parsePuzzle(std::fstream &&istream) {
  auto t = HolidayBag::SportTimer("Parsing", "us");
  std::vector<std::string> boardingPasses{};

  for (std::string s{}; std::getline(istream, s, '\n');) {
    if (!s.empty() && s.back() == '\r') {
      s.pop_back();
    }
    boardingPasses.emplace_back(s);
  }

  return boardingPasses;
}

int main(int argc, char *argv[]) {
  auto t = HolidayBag::SportTimer("Total", "us");

  std::cout << std::string(79, '-') << '\n';
  std::cout << "AoC2020_05 v1 & v2\n";
  if (argc != 2) {
    std::cerr << "Not enough (" << argc << ") arguments\n";
    return -1;
  }

  auto puzzle = std::fstream(argv[1], std::ios_base::in);
  if (!puzzle.is_open()) {
    std::cerr << "File '" << argv[1] << "' cannot be open\n";
    return -2;
  }

  auto boardingPasses = parsePuzzle(std::move(puzzle));
  std::cout << "Answer v1: " << crack_v1(boardingPasses) << '\n';
  std::cout << "Answer v2: " << crack_v2(boardingPasses) << '\n';

  t.stop();
  std::cout << t.getInterSummaryBag().unknit();
}
