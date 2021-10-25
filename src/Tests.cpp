#include "CSVReader.h"
#include <cassert>
struct Test {
	void importTable(const string& path, std::ostringstream& out) {
		CSVReader csv_reader;
		try {
			csv_reader.readTable("../tests/" + path);
			csv_reader.printTable(out);
		}
		catch (std::exception& e) {
			out << e.what();
		}
	}
};
void runTest(const string& path, const string& expr) {
	Test t;
	std::ostringstream out;
	t.importTable(path, out);
	cout << "PASSED: " << path << " : " << expr << "\n";
	assert(out.str() == expr);
	out.str(std::string());
}
void runTests() {
	//	 , A, B,      Cell
	//	1, 2, 1,     -1
	//	2, 2, =B1+A1, 0
	//	0, 4, 5			   <-- missing row value
	runTest("-1r.csv", "Invalid table\n");

	//	  ,  C
	//	 1,  1
	//	 2,  2
	//	30,  3
	// OK
	runTest("1col.csv", ",C\n1,1\n2,2\n30,3\n");

	//	 ,  A, B,      Cell
	//	1,  0, 1,      -1		
	//	2,  2, =B1/A1, 0
	//	30, 0, 4,      5
	runTest("divisionzero.csv", "Devision by zero in cells: B1 A1\n");

	//	  ,  A, A,      Cell
	//	 1,  2, 1,      -1
	//	 2,  2, =B1+A1, 0
	//	30,  0, 4,	    5
	runTest("duplicateC.csv", "Duplicate columns: A\n");	

	//	  , A, B,      Cell
	//	 2, 2, 1,      -1
	//	 2, 2, =B1+A1, 0
	//	30, 0, 4,      5
	runTest("duplicateR.csv", "Duplicate rows: 2\n");
	
	// empty file
	runTest("empty.csv", "File is empty\n");	

	//	  ,  A, B, Cell
	//	 1, 10, ,  -1			<-- B1 and B2 cells are empty
	//	 2,  2, ,  0			
	//	30,  0, 4, 5
	runTest("emptycell.csv", "Empty value\n");
	
	//					<-- no column names
	//	 1, 2, 1, -1	
	//	 2, 2, 1, 0
	//	30, 0, 4, 5
	runTest("nocols.csv", "Column name can only be char");

	// , A, B, Cell
	//					<-- only col names
	// OK
	runTest("norows.csv", ",A,B,Cell\n");

	// wrong format css
	runTest("notcsv.css", "File is not '.csv'\n");

	//	 , A, B, Cell
	//	1, 2, 1, -1
	// OK
	runTest("onerow.csv", ",A,B,Cell\n1,2,1,-1\n");
	
	// 
	//	1
	//	2
	//	30
	// OK
	runTest("onlyIds.csv", "\n1\n2\n30\n");

	//	  , A,       B	 _    <-- missing col name
	//	 1, 2,       1,  -1
	//	 2, 2,  =B1+A1,  0
	//	30, 0,       4,  5
	runTest("wcol.csv", "Invalid table\n");

	//	  ,  A,     B,    Cell
	//	 1, 10,     1,    -1
	//	 2,  2, B1/A1,	   0	<-- wrong cell expression (no '=')
	//	30,  0,     4,     5
	runTest("wrongcell.csv", "Cell can be only digit or expression\n");

	//	  ,  A,    B,  Cell
	//	 1, 10,    1, -1
	//	 2,  2,  /A1,  0    <-- wrong cell expression (no '=(OP)')
	//	30,  0,    4,  5
	runTest("wrongcell2.csv", "Cell can be only digit or expression\n");

	//	  ,  A,   B,   Cell
	//	 1, 10,   1,  -1
	//	 2,  2, =B1,   0	   <-- wrong cell expression (no '+(OP)')
	//	30,  0,   4,   5
	runTest("wrongcell3.csv", "Wrong expression: =B1\n");	

	//	  ,  A,   B,   Cell
	//	 1, 10,   1,  -1
	//	 2,  2, =Ba,   0		<-- wrong cell expression (no 'Ba')
	//	30,  0,   4,   5
	runTest("wrongcell4.csv", "Wrong expression: =Ba\n");

	//	  ,  A,   B,   Cell
	//	1a, 10,   1,   -1		<-- wrong row name (1a)
	//	 2,  2,   2,   0    
	//	30,  0,   4,   5
	runTest("wrongrow.csv", "Cell can be only digit or expression\n");

	//	  , A, B, Cell
	//	-1, 2, 1, -1      <- worng row name (-1)
	//	 2, 2, 1, 0
	//	30, 0, 4, 5
	runTest("negativerow.csv", "Id can be only positive integers in rows id: -1\n");
	
	//	, A, B, Cell
	// 1, 2, 1, -1
	// 2, 2, 3, 0
	// 30, 0, 4, 5
	// OK
	runTest("valid1.csv", ",A,B,Cell\n1,2,1,-1\n2,2,3,0\n30,0,4,5\n");

	//   ,  A, B, Cell
	//  1, -1, 1, -1
	//  2,  4, 5, 0
	// 30,  0, 4, 5
	// OK
	runTest("valid2.csv", ",A,B,Cell\n1,-1,1,-1\n2,4,5,0\n30,0,4,5\n");
}