# CSVReader

# How to build and run

Cmake:

```
git clone https://github.com/Genomorf/CSVReader
cd CSVReader/
mkdir build
cd build
cmake ..
make CSVReader
```

Пример работы:

```
./CSVReader ../tests/valid1.csv
```

g++
```
g++ -std=c++17 main.cpp ErrorCheckers.h ErrorCheckers.cpp CSVReader.h CSVReader.cpp -lstdc++fs
```

Пример работы:
```
./a.out ../tests/valid1.csv
```