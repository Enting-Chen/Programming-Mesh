#include "../inc/TestHarness.h"
#include "Mesh2Dquad.h"
#include <iostream>
using namespace std;


int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.³Â¶÷æÃ.Mesh01";  //Change to your ID and name
bool submission			= true;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"Mesh2Dquad.cpp",
	"Mesh2Dquad.h",
	"Mesh2DquadTest.cpp",
	""
};


TEST(Meshr, creation ,9)
{
	Mesh2Dquad s;
	s.read("new_grid.dat");

	CHECK_LONGS_EQUAL(s.cell[100000], 25041, 10);
	CHECK_LONGS_EQUAL(s.cell[100001], 25042, 10);
	CHECK_LONGS_EQUAL(s.cell[100002], 25643, 10);
	CHECK_LONGS_EQUAL(s.cell[100003], 25642, 10);
	CHECK_LONGS_EQUAL(s.cell[100004], 25042, 10);

	s.save("test.grid");

	//Mesh2Dquad s1;
	//s.read("test.grid");

	CHECK_LONGS_EQUAL(s.edge[100000], 24962, 10);
	CHECK_LONGS_EQUAL(s.edge[100001], 25563, 10);
	CHECK_LONGS_EQUAL(s.edge[100002], 25563, 10);
	CHECK_LONGS_EQUAL(s.edge[100003], 25564, 10);
	CHECK_LONGS_EQUAL(s.edge[100004], 24963, 10);
}
