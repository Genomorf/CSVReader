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

>,A,B,Cell
 1,2,1,-1
 2,2,3,0
 30,0,4,5
```
g++
```
git clone https://github.com/Genomorf/CSVReader
cd CSVReader/src
g++ -std=c++17 main.cpp ErrorCheckers.h ErrorCheckers.cpp CSVReader.h CSVReader.cpp -lstdc++fs
```
Пример работы:
```
./a.out ../tests/valid1.csv

>,A,B,Cell
 1,2,1,-1
 2,2,3,0
 30,0,4,5
```