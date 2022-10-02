#include "../inc/TestHarness.h"
#include "MeshVTK3Alg.h"
#include <iostream>
using namespace std;


int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.陈恩婷.Mesh04";  //Change to your ID and name
bool submission			= true;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"MeshVTK3Alg.h",
	"MeshVTK3Alg.cpp",
	"MeshVTK3AlgTest.cpp",
	""
};


TEST(Meshr, creation ,9)
{
/*	MeshVTK s;
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
	*/
	
	typedef double real;
	// global constants

	//real gam, gm1, cfl, eps, mach, alpha, qinf[4];
	MeshVTK s;
	s.read("grid.vtk");
	s.save("1.vtk");
	s.initflow();
	int niter = 100;
	//double save = 0, area = 0, update = 0, flux_res = 0, perem = 0, wall_t_a = 0, wall_t_b = 0;
	for (int iter = 1; iter <= niter; iter++) {
		for (int i = 0; i < s.ncell; i++) {
			s.sav( i);
		}
		for (int i = 0; i < s.ncell; i++) {
			s.area( i);
		}
		for (int i = 0; i < s.nedge; i++) {
			s.flux( i);
		}
		for (int i = 0; i < s.nbedge; i++) {
			s.bcond( i);
		}

		for (int i = 0; i < s.ncell; i++) {
			s.update( i);
		}

		s.rms = sqrt(s.rms / (double)s.ncell);
		if (iter % 10 == 0) {
			printf(" %d  %10.5e \n", iter, s.rms);
			//printf("\tsave: %f\n", save);
			//printf("\tarea: %f\n", area);
			//printf("\tflux_res: %f\n", flux_res);
			//printf("\tperem: %f\n", perem);
			//printf("\tupdate: %f\n", update);
			char buf[50];
			sprintf(buf, "out%d.vtk", iter);
			//WriteMeshToVTKAscii(buf, x, nnode, cell, ncell, q);
		}

	}

}
