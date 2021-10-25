#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <sstream>
#include <fstream>
#include <regex>
#include <filesystem>
#include <cctype>
using std::move;
using std::vector;
using std::map;
using std::string;

namespace ErrorCheckers {
    void isIdCanBeStoi(string& Id);
    void isTableHasDuplicateIds(const vector<string>& rows_ids, const string& tmp);
    void isTableHasDuplicateColumns(const vector<string>& column_names, const string& tmp);
    void isColumnsNamesHasDigits(const vector<string>& column_names);
    void isAmountOfRowsAreEqual(const vector<string>& rows);
    void isValueEmpty(const string& tmp);
    void pathValidation(const string& path);
};