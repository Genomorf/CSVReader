#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <cmath>
#include <numeric>
#include <sstream>
#include <bitset>
#include <set>
#include <thread>
#include <mutex>
#include <fstream>
#include <regex>
#include <filesystem>
using namespace std;


struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};


bool isSubsequence(string s1, string s2) {
    int i = 0;
    while (i != s2.size()) {
        if (s1.find(s2[i]) == string::npos) {
            s2.erase(s2.begin() + i);
        }
        else {
            ++i;
        }
    }
    return s2.find(s1) != string::npos;
}
template<typename T>
std::ostream& operator<<(std::ostream& out, std::vector<T>& vec) {
    for (const auto& i : vec) {
        out << i << " ";
    }
    out << std::endl;
    return out;
}

template<typename T, typename U>
void printMap(map<T, U>& m) {
    for (auto i : m) {
        cout << i.first << " " << i.second << '\n';
    }
}





// duplicate rows 0

class CSVReader {
public:
    string data_;
    vector<string> column_names;
    vector<string> rows;
    map<string, string> cells;
    vector<string> rows_ids;

    void readTable(const string& path) {
        filesystem::path p = path;
        if (p.extension() != ".csv") {
            cout << "File is not '.csv' ";
        }
        if (filesystem::is_empty(p)) {
            cout << "em";
        }
        ifstream in(path);
        if (!in.is_open()) {
            throw std::exception{ "Couldn't open file" };
        }
        string tmp((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        in.close();
        data_ = move(tmp);
        data_.insert(0, "0");
        getCells();
        calculateCellsExpressions();
        
    }
    void printTable() const {

        for (const auto& i : cells) {
            cout << i.first << " " << i.second << "\n";
        }
        cout << '\n';
        for (const auto& i : rows_ids) {
            for (const auto& j : column_names) {
                if(j != "0" || i !="0")
                //cout << cells[j + i];
                    cout << cells.at(j + i);
                if (&j != &column_names.back())
                    cout << ",";
            }
            cout << '\n';
        }
    }
    void getColumnNames() {
        // split data by '\n' and get first row == head
        stringstream in_(data_);
        string head;
        getline(in_, head, '\n');
        // split head by ',' and get col names
        string tmp;
        stringstream s(head);
        
        while (getline(s, tmp, ',')) {
            if (find(column_names.begin(), column_names.end(), tmp) != column_names.end()) {
                string s=  {"Duplicate columns: " + tmp + '\n' };
                cout << s;
                //throw std::exception{ s.c_str() };
            }
            column_names.push_back(tmp);
        }
        cout << column_names;
        cout << '\n';
    }
    void checkIfSameColumns(const string& col) {

    }
    void getRows() {
        stringstream in_(data_);
        string tmp;
        while (getline(in_, tmp, '\n')) {
            rows.push_back(tmp);
        }
        cout << rows;
        cout << '\n';
    }

    void getCells() {

        getRows();
        getColumnNames();
        getRowsIds();

        string tmp;
        string row;
        string row_number;

        for (const auto& i : rows) {
            tmp.clear();
            row = i;
            stringstream s(row); 
            getline(s, row_number, ',');
            cells["0" + row_number] = row_number; 
            size_t counter = 1;
            // split row by ',' and write to map each cell 
            // key: col name + row name, e.g. A1 or B3. Value: cell value
            while (getline(s, tmp, ',')) {
                if (tmp.empty()) {
                    string s = { "Emty value\n" };
                    cout << s;
                }
                cells[column_names[counter] + row_number] = tmp;
                ++counter;
            }
        }
    }
    void calculateCell(string& cell) {
        regex r("=(\\w+)([\\+\\*\\-/])(\\w+)");
        smatch m;
        if (cell[0] == '=') {

            regex_search(cell, m, r);
            if (cells.find(m[1]) == cells.end() ||
                cells.find(m[3]) == cells.end()) {
                string s {"Cell " + m[1].str() + ", " + m[3].str() + " isn't exist\n" };
                throw std::exception{ s.c_str() };
            }
            if (cells[m[1]][0] == '=') {
                calculateCell(cells[m[1]]);
            }            
            if (cells[m[3]][0] == '=') {
                calculateCell(cells[m[3]]);
            }
            int left = 0;
            int right = 0;
            try {
                left = stoi(cells[m[1]]);
                right = stoi(cells[m[3]]);
            }
            catch (std::exception& e) {
                string s{ e.what() };
                s += " in cells: " + m[1].str() + " " + m[3].str() + '\n';
                throw std::exception{ s.c_str() };
            }
            if (m[2] == "+") {
                cell = to_string(left + right);
            }
            else if (m[2] == "-") {
                cell = to_string(left - right);
            }
            else if (m[2] == "*") {
                cell = to_string(left * right);
            }
            else if (m[2] == "/") {
                if (right == 0) { 
                    string s{ "Devision by zero in cells: " + m[1].str() + " " + m[3].str() + '\n'};
                    throw std::domain_error{ s }; }
                cell = to_string(left / right);
            }
        }
    }
    void calculateCellsExpressions() {
        for (auto& i : cells) {
            try {
                calculateCell(i.second);
            }
            catch (std::exception& e) {
                cout << e.what();
            }
        }
    }
    void getRowsIds() {
        string tmp;
        rows_ids.reserve(rows.size());
        for (const auto& i : rows) {
            tmp.clear();
            stringstream s(i);
            getline(s, tmp, ',');
            checkRowsId(tmp);
            if (find(rows_ids.begin(), rows_ids.end(), tmp) != rows_ids.end()) {
                string s = { "Duplicate rows: " + tmp + '\n' };
                cout << s;
            }
            rows_ids.push_back(tmp);
        }
        cout << rows_ids;
        cout << '\n';

    }
    void checkRowsId(string& Id) {
        try {
            stoi(Id);
        }
        catch (std::exception& e) {
            string s{ e.what() };
            s += " in rows id: " + Id + '\n';
            cout << s;
            //throw std::exception{ s.c_str() };
        }
    }
};
int main() {
    CSVReader r;
    r.readTable("file.csv");
    try {
        r.printTable();
    }
    catch (std::exception& e) {
        cout << e.what();
    }

}


