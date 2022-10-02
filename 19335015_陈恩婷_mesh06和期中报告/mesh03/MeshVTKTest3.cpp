#include "../inc/TestHarness.h"
#include "MeshVTK3.h"
#include <iostream>
using namespace std;


int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.³Â¶÷æÃ.Mesh03";  //Change to your ID and name
bool submission			= true;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"MeshVTK3.cpp",
	"MeshVTK3.h",
	"MeshVTKTest3.cpp",
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
	CHECK_LONGS_EQUAL(s.celltypes[0], 1, 5);
	CHECK_LONGS_EQUAL(s.celltypes[10], 3, 5);
	CHECK_LONGS_EQUAL(s.celltypes[23], 5, 5);
	CHECK_LONGS_EQUAL(s.csize, 24, 5);
	CHECK_LONGS_EQUAL(s.celldata[23], 6, 5);
	CHECK_LONGS_EQUAL(s.celldata[10], 5, 5);
	CHECK_LONGS_EQUAL(s.celldata[0], -1, 5);

	s.save("test.vtk");
	MeshVTK s1;
	s1.read("test.vtk");
	CHECK_LONGS_EQUAL(s1.npoint, 11, 5);
	CHECK_LONGS_EQUAL(s1.ncell, 24, 5);
	CHECK_LONGS_EQUAL(s1.cellsize, 80, 5);
	CHECK_LONGS_EQUAL(s1.cells[7], 3, 5);
	CHECK_LONGS_EQUAL(s1.cells[74], 6, 5);
	CHECK_LONGS_EQUAL(s1.cells[79], 10, 5);
	CHECK_LONGS_EQUAL(s1.celltypes[0], 1, 5);
	CHECK_LONGS_EQUAL(s1.celltypes[10], 3, 5);
	CHECK_LONGS_EQUAL(s1.celltypes[23], 5, 5);
	CHECK_LONGS_EQUAL(s1.csize, 24, 5);
	CHECK_LONGS_EQUAL(s1.celldata[23], 6, 5);
	CHECK_LONGS_EQUAL(s1.celldata[10], 5, 5);
	CHECK_LONGS_EQUAL(s1.celldata[0], -1, 5);
	
	MeshVTK s2;
	s2.read("cube.vtk");
	CHECK_LONGS_EQUAL(s2.npoint, 141, 5);
	CHECK_LONGS_EQUAL(s2.ncell, 689, 5);
	CHECK_LONGS_EQUAL(s2.cellsize, 3065, 5);
	CHECK_LONGS_EQUAL(s2.cells[7], 3, 5);
	CHECK_LONGS_EQUAL(s2.cells[74], 22, 5);
	CHECK_LONGS_EQUAL(s2.cells[79], 2, 5);
	CHECK_LONGS_EQUAL(s2.celltypes[0], 1, 5);
	CHECK_LONGS_EQUAL(s2.celltypes[10], 3, 5);
	CHECK_LONGS_EQUAL(s2.celltypes[510], 10, 5);
	CHECK_LONGS_EQUAL(s2.celltypes[688], 10, 5);
	CHECK_LONGS_EQUAL(s2.csize, 689, 5);
	CHECK_LONGS_EQUAL(s2.celldata[623], 1, 5);
	CHECK_LONGS_EQUAL(s2.celldata[624], 1, 5);
	CHECK_LONGS_EQUAL(s2.celldata[0], -1, 5);

}
