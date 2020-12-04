// Copyright 2020 HalfsInner
#include <algorithm>
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
auto crack_v1(Container &&passports) {
  auto t = HolidayBag::SportTimer("p1", "us", 1);
  return std::accumulate(cbegin(passports), cend(passports), 0u, [](auto &&sum, auto &&passport) {
    auto isValid = (passport.size() == 8) ||
                   (passport.size() == 7 && (passport.find("cid") == cend(passport)));
    return isValid ? sum + 1 : sum;
  });
}

template <typename Container>
auto crack_v2(Container &&passports) {
  auto t = HolidayBag::SportTimer("p2", "us", 1);
  return std::accumulate(cbegin(passports), cend(passports), 0u, [](auto &&sum, auto &&passport) {
    auto isValidFieldNumber = (passport.size() == 8) ||
                              (passport.size() == 7 && (passport.find("cid") == cend(passport)));
    if (!isValidFieldNumber) {
      return sum;
    }

    auto isValidByr = false;
    if (auto it = passport.find("byr"); it != cend(passport)) {
      const auto year = std::stoi(it->second);
      isValidByr = year >= 1920 && year <= 2002;
    }

    auto isValidIyr = false;
    if (auto it = passport.find("iyr"); it != cend(passport)) {
      const auto year = std::stoi(it->second);
      isValidIyr = year >= 2010 && year <= 2020;
    }

    auto isValidEyr = false;
    if (auto it = passport.find("eyr"); it != cend(passport)) {
      const auto year = std::stoi(it->second);
      isValidEyr = year >= 2020 && year <= 2030;
    }

    auto isValidHgt = false;
    if (auto it = passport.find("hgt"); it != cend(passport)) {
      const auto &hgt = it->second;
      if (auto pos = hgt.find("cm"); pos != std::string::npos) {
        const auto height = std::stoi(hgt.substr(0, pos));
        isValidHgt = height >= 150 && height <= 193;
      } else if (auto pos = hgt.find("in"); pos != std::string::npos) {
        const auto number = std::stoi(hgt.substr(0, pos));
        isValidHgt = number >= 59 && number <= 76;
      }
    }

    auto isValidHcl = false;
    if (auto it = passport.find("hcl"); it != cend(passport)) {
      const auto &hair = it->second;
      isValidHcl = hair.front() == '#' &&
                   std::find_if_not(cbegin(hair) + 1, cend(hair), ::isxdigit) == cend(hair);
    }

    auto isValidEcl = false;
    if (auto it = passport.find("ecl"); it != cend(passport)) {
      const std::unordered_set<std::string> legalEyes{"amb", "blu", "brn", "gry",
                                                      "grn", "hzl", "oth"};
      isValidEcl = legalEyes.find(it->second) != end(legalEyes);
    }

    auto isValidPid = false;
    if (auto it = passport.find("pid"); it != cend(passport)) {
      const auto &pid = it->second;
      isValidPid =
          pid.size() == 9 && std::find_if_not(cbegin(pid), cend(pid), ::isdigit) == cend(pid);
    }

    const auto isValid = isValidFieldNumber && isValidByr && isValidIyr && isValidEyr &&
                         isValidHgt && isValidHcl && isValidEcl && isValidPid;
    return isValid ? sum + 1 : sum;
  });
}

auto parsePuzzle(std::fstream &&istream) {
  auto t = HolidayBag::SportTimer("Parsing", "us");
  std::vector<std::unordered_map<std::string, std::string>> passports{};

  std::regex passportField("(\\w+):(#?\\w+)");
  for (std::string s{}; std::getline(istream, s, '\n');) {
    if (!s.empty() && s.back() == '\r') {
      s.pop_back();
    }
    if (passports.empty() || s.empty()) {
      passports.push_back({});
    }
    auto &lastPassport = passports.back();
    for (std::sregex_iterator it(s.begin(), s.end(), passportField), it_end; it != it_end; ++it) {
      auto &&key = (*it)[1];
      auto &&value = (*it)[2];
      lastPassport[key] = value;
    }
  }

  return passports;
}

int main(int argc, char *argv[]) {
  auto t = HolidayBag::SportTimer("Total", "us");

  std::cout << std::string(79, '-') << '\n';
  std::cout << "AoC2020_04 v1 & v2\n";
  if (argc != 2) {
    std::cerr << "Not enough (" << argc << ") arguments\n";
    return -1;
  }

  auto puzzle = std::fstream(argv[1], std::ios_base::in);
  if (!puzzle.is_open()) {
    std::cerr << "File '" << argv[1] << "' cannot be open\n";
    return -2;
  }

  auto passports = parsePuzzle(std::move(puzzle));
  std::cout << "Answer v1: " << crack_v1(passports) << '\n';
  std::cout << "Answer v2: " << crack_v2(passports) << '\n';

  t.stop();
  std::cout << t.getInterSummaryBag().unknit();
}