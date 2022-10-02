#include "../inc/TestHarness.h"
#include "MeshVTK.h"
#include <iostream>
using namespace std;


int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.³Â¶÷æÃ.Mesh02";  //Change to your ID and name
bool submission			= true;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"MeshVTK.cpp",
	"MeshVTK.h",
	"MeshVTKTest.cpp",
	""
};


TEST(Meshr, creation ,9)
{
	MeshVTK s;
	s.read("t1.vtk");

	CHECK_LONGS_EQUAL(s.npoint, 11, 5);
	CHECK_LONGS_EQUAL(s.ncell, 24, 5);
	CHECK_LONGS_EQUAL(s.cellsize, 80, 5);
	CHECK_LONGS_EQUAL(s.cells[7], 3, 5);
	CHECK_LONGS_EQUAL(s.cells[74], 6, 5);
	CHECK_LONGS_EQUAL(s.cells[79], 10, 5);
	CHECK_LONGS_EQUAL(s.celltype[0], 1, 5);
	CHECK_LONGS_EQUAL(s.celltype[10], 3, 5);
	CHECK_LONGS_EQUAL(s.celltype[23], 5, 5);
	CHECK_LONGS_EQUAL(s.q[23], 6, 5);
	CHECK_LONGS_EQUAL(s.q[10], 5, 5);
	CHECK_LONGS_EQUAL(s.q[0], -1, 5);

	s.save("test.vtk");

	CHECK_LONGS_EQUAL(s.npoint, 11, 5);
	CHECK_LONGS_EQUAL(s.ncell, 24, 5);
	CHECK_LONGS_EQUAL(s.cellsize, 80, 5);
	CHECK_LONGS_EQUAL(s.cells[7], 3, 5);
	CHECK_LONGS_EQUAL(s.cells[74], 6, 5);
	CHECK_LONGS_EQUAL(s.cells[79], 10, 5);
	CHECK_LONGS_EQUAL(s.celltype[0], 1, 5);
	CHECK_LONGS_EQUAL(s.celltype[10], 3, 5);

}
