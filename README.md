# AoC2020
Solutions of AoC2020. Written in C++.

## Build and Execute
Build includes all solutions.

### Locally
```sh
mkdir build && cd build
cmake .. && make -j 8
aoc_XX/aoc_XX ../aoc_XX/input.txt # where XX is a number of puzzle
```

### Docker
```sh
docker build -t aoc_2020:latest .
docker container run aoc_2020
```

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Author
- **Half's Inner**