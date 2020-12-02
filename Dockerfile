FROM ubuntu
RUN apt-get -y update && apt-get -y install g++ cmake

COPY . /app
RUN cd app && mkdir build && cd build &&\
    cmake .. && make

CMD /app/build/aoc_01/aoc_01 /app/aoc_01/input.txt &&\
    /app/build/aoc_02/aoc_02 /app/aoc_02/input.txt