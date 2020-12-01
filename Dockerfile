FROM ubuntu
COPY . /app
RUN apt-get -y update && apt-get -y install g++ cmake &&\
    cd app && mkdir build && cd build &&\
    cmake .. && make
CMD /app/build/aoc_01/aoc_01 /app/aoc_01/input.txt