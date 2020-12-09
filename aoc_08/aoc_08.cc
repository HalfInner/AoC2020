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

class Machine {
    int _pc = 0;
    int _acc = 0;
    bool _isRunning = false;

    std::vector<bool> _visitedPc;
    std::vector<std::pair<std::string, int>> _program;

    bool _isRepetitionExists = false;

   public:
    Machine(std::vector<std::pair<std::string, int>> program) {
        _visitedPc.resize(program.size());
        _program = std::move(program);
        _isRunning = true;
    }

    bool step() {
        if (!_isRunning) {
            return _isRunning;
        }
        if (_pc == static_cast<int>(_program.size())) {
            _isRunning = false;
            return _isRunning;
        }
        if (_visitedPc.at(_pc)) {
            _isRunning = false;
            _isRepetitionExists = true;
            return _isRunning;
        }
        auto &&[cmd, val] = _program.at(_pc);
        _visitedPc[_pc] = true;

        if (cmd == "nop") {
            ++_pc;
        } else if (cmd == "acc") {
            ++_pc;
            _acc += val;
        } else if (cmd == "jmp") {
            _pc += val;
        } else {
            throw std::runtime_error("No command was found: " + cmd);
        }

        return _isRunning;
    }

    int accumulator() const { return _acc; }
    int isRepetitionExists() const { return _isRepetitionExists; }
};

template <typename Container>
auto crack_v1(Container &&rawProgram) {
    auto t = HolidayBag::SportTimer("p1", "us", 1);
    Machine m(rawProgram);

    for (; m.step();) { }

    return m.accumulator();
}

template <typename Container>
auto crack_v2(Container &&rawProgram) {
    auto t = HolidayBag::SportTimer("p2", "us", 1);

    for (auto &&el : rawProgram) {
        if (el.first == "nop") {
            el.first = "jmp";
        } else if (el.first == "jmp") {
            el.first = "nop";
        }

        Machine m(rawProgram);
        for (; m.step();) { }

        if (!m.isRepetitionExists()) {
            return m.accumulator();
        }

        if (el.first == "nop") {
            el.first = "jmp";
        } else if (el.first == "jmp") {
            el.first = "nop";
        }
    }

    return -1;
}

auto parsePuzzle(std::fstream &&istream) {
    auto t = HolidayBag::SportTimer("Parsing", "us");

    std::vector<std::pair<std::string, int>> rawProgram;
    for (std::string s {}; std::getline(istream, s, '\n');) {
        if (!s.empty() && s.back() == '\r') {
            s.pop_back();
        }
        size_t pos = s.find(' ');
        auto cmd = s.substr(0, pos);
        auto val = std::stoi(s.substr(pos + 1));
        rawProgram.emplace_back(cmd, val);
    }

    return rawProgram;
}

int main(int argc, char *argv[]) {
    auto t = HolidayBag::SportTimer("Total", "us");

    std::cout << std::string(79, '-') << '\n';
    std::cout << "AoC2020_08 v1 & v2\n";
    if (argc != 2) {
        std::cerr << "Not enough (" << argc << ") arguments\n";
        return -1;
    }

    auto puzzle = std::fstream(argv[1], std::ios_base::in);
    if (!puzzle.is_open()) {
        std::cerr << "File '" << argv[1] << "' cannot be open\n";
        return -2;
    }

    auto rawProgram = parsePuzzle(std::move(puzzle));
    std::cout << "Answer v1: " << crack_v1(rawProgram) << '\n';
    std::cout << "Answer v2: " << crack_v2(rawProgram) << '\n';

    t.stop();
    std::cout << t.getInterSummaryBag().unknit();
}
