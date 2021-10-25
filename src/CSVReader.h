#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <fstream>
#include <regex>
#include <filesystem>
#include <cctype>
#include "ErrorCheckers.h"

using std::cout;
using std::move;
using std::vector;
using std::map;
using std::string;

class CSVReader {
private:
    string data_;
    vector<string> column_names;
    vector<string> rows;
    map<string, string> cells;
    vector<string> rows_ids;

    void getColumnNames();

    void getRows();

    void getCells();

    void areCellsHaveArgs(const string& left, const string& right, const string& cell);
    
    std::pair<int, int> stoiArgs(const string& left_s, const string& right_s);

    void calculateCell(string& cell);

    bool checkIfDigit(const string& s);

    void calculateCellsExpressions();

    void getRowsIds();

public:
    void readTable(const string& path);

    void printTable(std::ostream& out) const;
};