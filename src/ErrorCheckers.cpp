#include "ErrorCheckers.h"


    void ErrorCheckers::isIdCanBeStoi(string& Id) {
        try {
            int Id_i = stoi(Id);
            if (Id_i <= 0) {
                throw std::exception{ "Id can be only positive integers" };
            }
        }
        catch (std::exception& e) {
            string s{ e.what() };
            s += " in rows id: " + Id + '\n';
            throw std::exception{ s.c_str() };
        }
    }
    void ErrorCheckers::isTableHasDuplicateIds(const vector<string>& rows_ids, const string& tmp) {
        if (find(rows_ids.begin(), rows_ids.end(), tmp) != rows_ids.end()) {
            string s = { "Duplicate rows: " + tmp + '\n' };
            throw std::exception{ s.c_str() };
        }
    }
    void ErrorCheckers::isTableHasDuplicateColumns(const vector<string>& column_names, const string& tmp) {
        if (find(column_names.begin(), column_names.end(), tmp) != column_names.end()) {
            string s = { "Duplicate columns: " + tmp + '\n' };
            throw std::exception{ s.c_str() };
        }
    }
    void ErrorCheckers::isColumnsNamesHasDigits(const vector<string>& column_names) {
        for (auto it = column_names.begin() + 1; it != column_names.end(); ++it) {
            if (!isalpha((*it)[0])) {
                throw std::exception{ "Column name can only be char" };
            }
        }
    }
    void ErrorCheckers::isAmountOfRowsAreEqual(const vector<string>& rows) {
        int counter = count(rows[0].begin(), rows[0].end(), ',');
        for (const auto& i : rows) {
            int tmp = count(i.begin(), i.end(), ',');
            if (tmp != counter) throw std::exception{ "Invalid table\n" };
            counter = tmp;
        }
    }
    void ErrorCheckers::isValueEmpty(const string& tmp) {
        if (tmp.empty()) {
            string s = { "Empty value\n" };
            throw std::exception{ s.c_str() };
        }
    }
    void ErrorCheckers::pathValidation(const string& path) {
        std::filesystem::path p = path;
        if (!std::filesystem::exists(p)) {
            throw std::exception{ "File does not exist\n" };
        }
        if (p.extension() != ".csv") {
            throw std::exception{ "File is not '.csv'\n" };
        }
        if (std::filesystem::is_empty(p)) {
            throw std::exception{ "File is empty\n" };
        }
    }
