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
#include <cctype>
using namespace std;

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
        if (!filesystem::exists(p)) {
            throw std::exception{ "File does not exist\n" };
        }
        if (p.extension() != ".csv") {
            throw std::exception{ "File is not '.csv'\n" };
        }
        if (filesystem::is_empty(p)) {
            throw std::exception{"File is empty\n"};
        }
        ifstream in(path);
        if (!in.is_open()) {
            throw std::exception{ "Couldn't open file\n" };
        }
        data_ = { (istreambuf_iterator<char>(in)), istreambuf_iterator<char>() };
        data_.insert(0, "0"); // add zero to first column - top left cell

        in.close();

        getCells();
        calculateCellsExpressions();
    }

    void printTable() const {
        for (const auto& i : rows_ids) {
            for (const auto& j : column_names) {
                if(j != "0" || i !="0") // skip first top left column value
                    cout << cells.at(j + i);
                if (&j != &column_names.back()) // skip last ','
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
            // check if table has duplicate columns
            if (find(column_names.begin(), column_names.end(), tmp) != column_names.end()) {
                string s=  {"Duplicate columns: " + tmp + '\n' };
                throw std::exception{ s.c_str() };
            }

            column_names.push_back(move(tmp));
        }

        for (auto it = column_names.begin() + 1; it != column_names.end(); ++it) {
            if (!isalpha((*it)[0])) {
                throw std::exception{ "Column name can only be char" };
            }
        }
    }

    void getRows() {
        stringstream in_(data_);
        string tmp;
        while (getline(in_, tmp, '\n')) {
            rows.push_back(move(tmp));
        }

        // check if numbers of columns are equal in every row
        int counter = count(rows[0].begin(), rows[0].end(), ',');
        for (const auto& i : rows) {
            int tmp = count(i.begin(), i.end(), ',');
            if (tmp != counter) throw std::exception{ "Invalid table\n" };
            counter = tmp;
        }
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
            cells["0" + row_number] = row_number; // get row number == Id

            size_t counter = 1;

            // split row by ',' and write to map each cell 
            // key: col name + row name, e.g. A1 or B3. Value: cell value
            while (getline(s, tmp, ',')) {
                if (tmp.empty()) {
                    string s = { "Emty value\n" };
                    throw std::exception{ s.c_str() };
                }

                cells[column_names[counter] + row_number] = move(tmp);
                ++counter;
            }
        }
       
    }
    void calculateCell(string& cell) {
        // if cell has expression "=A1+B1" - find it with regex,
        // cast to int, do the operation [+,*,-,/] and assign to cell
        regex r("=(\\w+)([\\+\\*\\-/])(\\w+)");
        smatch m;

        if (cell[0] == '=') {
            regex_search(cell, m, r);

            // check if expression is valid and both args are
            // exist in the table
            if (cells.find(m[1]) == cells.end() ||
                cells.find(m[3]) == cells.end()) {
                string s{ "Wrong expression: " + cell + "\n" };
                throw std::exception{ s.c_str() };
            }

            // if args refer not to int-cell but to another expression,
            // recursive calculate that expression
            if (cells[m[1]][0] == '=') {
                calculateCell(cells[m[1]]);
            }
            if (cells[m[3]][0] == '=') {
                calculateCell(cells[m[3]]);
            }

            // in "=A1+B1" left = A1, right = B1
            int left = 0;
            int right = 0;

            // catch error if std::stoi have failed
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
                    string s{ "Devision by zero in cells: " + m[1].str() + " " + m[3].str() + '\n' };
                    throw std::domain_error{ s };
                }
                cell = to_string(left / right);
            }
        }
        else if (!checkIfDigit(cell)) {
            throw std::exception{ "Cell can be only digit or expression\n" };
        }
        // if cell == number - do nothing
    }
    bool checkIfDigit(const string& s) {
        return s.find_first_not_of("-1234567890") == string::npos;
        
    }
    void calculateCellsExpressions() {
        for (auto& i : cells) {
            if(i.first != i.second + '0')
                calculateCell(i.second);
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

            // check if table has duplicate Ids
            if (find(rows_ids.begin(), rows_ids.end(), tmp) != rows_ids.end()) {
                string s = { "Duplicate rows: " + tmp + '\n' };
                throw std::exception{ s.c_str() };
            }

            rows_ids.push_back(move(tmp));
        }
    }
    void checkRowsId(string& Id) {
        try {
            stoi(Id);
        }
        catch (std::exception& e) {
            string s{ e.what() };
            s += " in rows id: " + Id + '\n';
            throw std::exception{ s.c_str() };
        }
    }
};
int main(int args, char* argv[]) {
    
    CSVReader r;
    try {
        r.readTable(argv[1]);
        //r.readTable("file.csv");
        r.printTable();
    }
    catch (std::exception& e) {
        cout << e.what();
    }
}


