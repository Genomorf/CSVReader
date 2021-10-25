#include "CSVReader.h"

//void runTests();
int main(int args, char* argv[]) {
    
    CSVReader r;
    try {
        r.readTable(argv[1]);
        r.printTable(cout);
    }
    catch (std::exception& e) {
        cout << e.what();
    }
    //runTests();
}


