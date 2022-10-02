#include "../inc/TestHarness.h"
#include "MeshMemory.h"
#include <iostream>
#include <stdlib.h>
using namespace std;


int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.³Â¶÷æÃ.Mesh07";  //Change to your ID and name
bool submission			= false;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"MeshMemory.h",
	"MeshMemory.cpp",
	"MeshMemoryTest.cpp",
	""
};

#ifdef CodeExecutable

TEST(Meshr, creation, 9)
{
	MeshMemory s;

	FILE* fp;
	if ((fp = fopen("t1.vtk", "r")) == NULL) {
		printf("can't open file\n");
		exit(-1);
	}
	char meshDescription[256];
	s.readvtkHeader(fp, meshDescription);

	int npoint;
	double* xyz;
	s.readvtkPoints(fp, npoint, xyz);

	int ncell, cellsize;
	int* cells;
	s.readvtkCells(fp, ncell, cellsize, cells);

	int* celltypes;
	s.readvtkCellTypes(fp, ncell, celltypes);

	double* celldata;
	int dimcell, ctype;
	s.readvtkCellData(fp, ncell, ctype, dimcell, celldata);

	int dimpoint, ptype;
	double* pointdata;
	s.readvtkPointData(fp, npoint, ptype, dimpoint, pointdata);

	Elements enodes = s.makeElements(npoint, "set_nodes");
	Elements ecells = s.makeElements(ncell, "set_cells");

	Map cell2point = s.makeMap(ecells, enodes, 4, cells, "cell_to_node_map");

	Data dcell1 = s.makeData(ecells, 1, "int", (char*)celltypes, "celltypes");
	Data dcell2 = s.makeData(ecells, 1, "double", (char*)celldata, "data_on_cells");
	Data dpoint1 = s.makeData(enodes, 3, "double", (char*)xyz, "nodes_xyz");
	Data dpoint2 = s.makeData(enodes, 1, "double", (char*)pointdata, "data_on_nodes");

	s.savetofile("t2.emd");

	double sum = 0, * pc = (double*)(dcell2->data);
	for (int i = 0; i < ncell; i++) {
		// cout << pc[i] << endl;
		sum += pc[i];
	}
	double psum = 0, * pd = (double*)(dpoint2->data);
	for (int i = 0; i < npoint; i++) {
		psum += pd[i];
	}
	double psum2 = 0;
	for (int i = 0; i < ncell; i++)
		for (int j = 0; j < cell2point->dim; j++) {
			// cout << i * cell2point->dim + j << " ";
			// cout << cell2point->map[i * cell2point->dim + j] << " ";
			// cout << pd[cell2point->map[i * cell2point->dim + j]] << endl;
			psum2 += pd[cell2point->map[i * cell2point->dim + j]];
		}

	CHECK_DOUBLES_EQUAL(sum, 2.662, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum, 76.8, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum2, 167.8, 0.0000001, 10);

	////////////////////////////////////////////////// Test 2

	MeshMemory s2("t2.emd");

	CHECK_LONGS_EQUAL(s2.element_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s2.map_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s2.dat_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s2.element_list_index, 2, 10);
	CHECK_LONGS_EQUAL(s2.map_list_index, 1, 10);
	CHECK_LONGS_EQUAL(s2.dat_list_index, 4, 10);

	sum = 0;
	pc = (double*)(s2.dat_list[1]->data);
	for (int i = 0; i < ncell; i++) {
		sum += pc[i];
	}

	psum = 0;
	pd = (double*)(s2.dat_list[3]->data);
	for (int i = 0; i < npoint; i++) {
		psum += pd[i];
	}

	Map mp = s2.map_list[0];
	psum2 = 0;
	for (int i = 0; i < ncell; i++){
	for (int j = 0; j < mp->dim; j++) {
		psum2 += pd[mp->map[i * mp->dim + j]];
	}
}
	CHECK_DOUBLES_EQUAL(sum, 2.662, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum, 76.8, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum2, 167.8, 0.0000001, 10);

	s2.savetofile("t3.emd");

	////////////////////////////////////////////////// Test 3

	MeshMemory s3("t2.emd");
	s3.readfromfile("t3.emd");

	CHECK_LONGS_EQUAL(s3.element_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s3.map_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s3.dat_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s3.element_list_index, 2, 10);
	CHECK_LONGS_EQUAL(s3.map_list_index, 1, 10);
	CHECK_LONGS_EQUAL(s3.dat_list_index, 4, 10);

	sum = 0;
	pc = (double*)(s3.dat_list[1]->data);
	for (int i = 0; i < ncell; i++) {
		sum += pc[i];
	}

	psum = 0;
	pd = (double*)(s3.dat_list[3]->data);
	for (int i = 0; i < npoint; i++) {
		psum += pd[i];
	}

	CHECK_DOUBLES_EQUAL(sum, 2.662, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum, 76.8, 0.0000001, 10);

	mp = s3.map_list[0]; 
	psum2 = 0;
	for (int i = 0; i < ncell; i++) {
		for (int j = 0; j < mp->dim; j++) {
			psum2 += pd[mp->map[i * mp->dim + j]];
		}
	}


	CHECK_DOUBLES_EQUAL(psum2, 167.8, 0.0000001, 10);


	////////////////////////////////////////////////// Test 4 

	MeshMemory s4("t3.emd");
	s4.init();

	if ((fp = fopen("t2.vtk", "r")) == NULL) {
		printf("can't open file\n");
		exit(-1);
	}
	free(xyz);
	free(cells);
	free(celltypes);
	free(celldata);
	free(pointdata);
	free(enodes);
	free(ecells);
	free(cell2point);
	free(dcell1);
	free(dcell2);
	free(dpoint1);
	free(dpoint2);
	s4.readvtkHeader(fp, meshDescription);
	s4.readvtkPoints(fp, npoint, xyz);
	s4.readvtkCells(fp, ncell, cellsize, cells);
	s4.readvtkCellTypes(fp, ncell, celltypes);
	s4.readvtkCellData(fp, ncell, ctype, dimcell, celldata);
	s4.readvtkPointData(fp, npoint, ptype, dimpoint, pointdata);

	enodes = s4.makeElements(npoint, "set_nodes");
	ecells = s4.makeElements(ncell, "set_cells");

	cell2point = s4.makeMap(ecells, enodes, 4, cells, "cell_to_node_map");

	dcell1 = s4.makeData(ecells, 1, "int", (char*)celltypes, "celltypes");
	dcell2 = s4.makeData(ecells, 1, "double", (char*)celldata, "data_on_cells");
	dpoint1 = s4.makeData(enodes, 3, "double", (char*)xyz, "nodes_xyz");
	dpoint2 = s4.makeData(enodes, 1, "double", (char*)pointdata, "data_on_nodes");
	s4.savetofile("t4.emd");
	sum = 0;
	pc = (double*)(dcell2->data);
	for (int i = 0; i < ncell; i++) {
		sum += pc[i];
	}
	psum = 0;
		pd = (double*)(dpoint2->data);
	for (int i = 0; i < npoint; i++) {
		psum += pd[i];
	}
	psum2 = 0;
	for (int i = 0; i < ncell; i++)
		for (int j = 0; j < cell2point->dim; j++) {
			psum2 += pd[cell2point->map[i * cell2point->dim + j]];
		}


	CHECK_LONGS_EQUAL(s4.element_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s4.map_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s4.dat_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s4.element_list_index, 2, 10);
	CHECK_LONGS_EQUAL(s4.map_list_index, 1, 10);
	CHECK_LONGS_EQUAL(s4.dat_list_index, 4, 10);

	CHECK_DOUBLES_EQUAL(sum, 2.914, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum, 97.0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum2, 200.7, 0.0000001, 10);


	////////////////////////////////////////////////// Test 5 

	MeshMemory s5("t4.emd");

	CHECK_LONGS_EQUAL(s5.element_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s5.map_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s5.dat_list_size, 10, 10);
	CHECK_LONGS_EQUAL(s5.element_list_index, 2, 10);
	CHECK_LONGS_EQUAL(s5.map_list_index, 1, 10);
	CHECK_LONGS_EQUAL(s5.dat_list_index, 4, 10);

	sum = 0;
	pc = (double*)(s5.dat_list[1]->data);
	for (int i = 0; i < ncell; i++) {
		sum += pc[i];
	}

	psum = 0;
	pd = (double*)(s5.dat_list[3]->data);
	for (int i = 0; i < npoint; i++) {
		psum += pd[i];
	}

	mp = s5.map_list[0];
	psum2 = 0;
	for (int i = 0; i < ncell; i++)
		for (int j = 0; j < mp->dim; j++) {
			psum2 += pd[mp->map[i * mp->dim + j]];
		}

	CHECK_DOUBLES_EQUAL(sum, 2.914, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum, 97.0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(psum2, 200.7, 0.0000001, 10);
	CHECK(strcmp(s5.dat_list[0]->type, "double") != 0,10);
}
#else
TEST(Meshr, creation, 9) {}
#endif