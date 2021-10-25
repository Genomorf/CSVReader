# CSVReader

# How to build and run

Ubuntu 20.04. Cmake:
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
Ubuntu 20.04, Windows 10 x64. g++ (с версии 9.0 так как есть модуль std::filesystem, который на ранних версиях не поддерживается).
```
git clone https://github.com/Genomorf/CSVReader
cd CSVReader/src
g++ -std=c++17 main.cpp ErrorCheckers.h ErrorCheckers.cpp CSVReader.h CSVReader.cpp -lstdc++fs
```
Пример работы:
```
./a.out ../tests/valid1.csv
or 
a.exe ../tests/valid1.csv

>,A,B,Cell
 1,2,1,-1
 2,2,3,0
 30,0,4,5
```
