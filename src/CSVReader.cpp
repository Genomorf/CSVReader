#include "CSVReader.h"

    void CSVReader::readTable(const string& path) {

        ErrorCheckers::pathValidation(path);

        std::ifstream in(path);
        if (!in.is_open()) {
            throw std::exception{ "Couldn't open file\n" };
        }
        data_ = { (std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>() };
        data_.insert(0, "0"); // add zero to first column - top left cell

        in.close();

        getCells();
        calculateCellsExpressions();
    }

    void CSVReader::printTable(std::ostream& out) const {
        for (const auto& i : rows_ids) {
            for (const auto& j : column_names) {
                if (j != "0" || i != "0") // skip first top left column value
                    out << cells.at(j + i);
                if (&j != &column_names.back()) // skip last ','
                    out << ",";
            }
            out << '\n';
        }
    }

    void CSVReader::getColumnNames() {
        // split data by '\n' and get first row == head
        std::stringstream in_(data_);
        string head;
        std::getline(in_, head, '\n');

        // split head by ',' and get col names
        string tmp;
        std::stringstream s(head);

        while (getline(s, tmp, ',')) {

            ErrorCheckers::isTableHasDuplicateColumns(column_names, tmp);

            column_names.push_back(move(tmp));
        }

        ErrorCheckers::isColumnsNamesHasDigits(column_names);
    }

    void CSVReader::getRows() {
        std::stringstream in_(data_);
        string tmp;
        while (getline(in_, tmp, '\n')) {
            rows.push_back(move(tmp));
        }

        ErrorCheckers::isAmountOfRowsAreEqual(rows);
    }

    void CSVReader::getCells() {

        getRows();
        getColumnNames();

        getRowsIds();

        string tmp;
        string row;
        string row_number;

        for (const auto& i : rows) {
            tmp.clear();
            row = i;
            std::stringstream s(row);
            getline(s, row_number, ',');
            cells["0" + row_number] = row_number; // get row number == Id

            size_t counter = 1;

            // split row by ',' and write to map each cell 
            // key: col name + row name, e.g. A1 or B3. Value: cell value
            while (getline(s, tmp, ',')) {
                ErrorCheckers::isValueEmpty(tmp);

                cells[column_names[counter] + row_number] = move(tmp);
                ++counter;
            }
        }

    }
    void CSVReader::areCellsHaveArgs(const string& left, const string& right, const string& cell) {
        if (cells.find(left) == cells.end() ||
            cells.find(right) == cells.end()) {
            string s{ "Wrong expression: " + cell + "\n" };
            throw std::exception{ s.c_str() };
        }
    }
    std::pair<int, int> CSVReader::stoiArgs(const string& left_s, const string& right_s) {
        try {
            int left = stoi(cells[left_s]);
            int right = stoi(cells[right_s]);
            return { left, right };
        }
        catch (std::exception& e) {
            string s{ e.what() };
            s += " in cells: " + left_s + " " + right_s + '\n';
            throw std::exception{ s.c_str() };
        }
    }
    void CSVReader::calculateCell(string& cell) {
        // if cell has expression "=A1+B1" - find it with regex,
        // cast to int, do the operation [+,*,-,/] and assign to cell
        std::regex r("=(\\w+)([\\+\\*\\-/])(\\w+)");
        std::smatch m;

        if (cell[0] == '=') {

            // in "=A1+B1" 
            // m[1] == "A1"
            // m[2] == "+"
            // m[3] == "B1"
            regex_search(cell, m, r);

            string first_op = m[1].str();
            string sec_op = m[3].str();
            string op = m[2].str();

            // check if expression is valid and both args are
            // exist in the table
            areCellsHaveArgs(first_op, sec_op, cell);

            // if args refer not to int-cell but to another expression,
            // recursive calculate that expression
            if (cells[first_op][0] == '=') {
                calculateCell(cells[first_op]);
            }
            if (cells[sec_op][0] == '=') {
                calculateCell(cells[sec_op]);
            }

            // catch error if std::stoi have failed
            // int left = (int)first_op
            // int right = (int)sec_op
            auto [left, right] = stoiArgs(first_op, sec_op);


            if (op == "+") {
                cell = std::to_string(left + right);
            }
            else if (op == "-") {
                cell = std::to_string(left - right);
            }
            else if (op == "*") {
                cell = std::to_string(left * right);
            }
            else if (op == "/") {
                if (right == 0) {
                    string s{ "Devision by zero in cells: " + first_op + " " + sec_op + '\n' };
                    throw std::domain_error{ s };
                }
                cell = std::to_string(left / right);
            }
        }
        else if (!checkIfDigit(cell)) {
            throw std::exception{ "Cell can be only digit or expression\n" };
        }
        // if cell == number - do nothing
    }
    bool CSVReader::checkIfDigit(const string& s) {
        return s.find_first_not_of("-1234567890") == string::npos;

    }
    void CSVReader::calculateCellsExpressions() {
        for (auto& i : cells) {
            if (i.first != i.second + '0')
                calculateCell(i.second);
        }
    }
    void CSVReader::getRowsIds() {
        string tmp;
        rows_ids.reserve(rows.size());
        for (const auto& i : rows) {
            tmp.clear();
            std::stringstream s(i);
            getline(s, tmp, ',');
            
            if(tmp[0] != rows[0][0]) // skip first zero in first column
                ErrorCheckers::isIdCanBeStoi(tmp);

            ErrorCheckers::isTableHasDuplicateIds(rows_ids, tmp);

            rows_ids.push_back(move(tmp));
        }
    }
