#include "../inc/TestHarness.h"
#include "MeshMemory.h"
#include "kernels.h"
#include <iostream>
using namespace std;


int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.陈恩婷.Mesh05";  //Change to your ID and name
bool submission = false;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"MeshMemory.h",
	"MeshMemory.cpp",
	"MeshMemoryTest.cpp",
	"kernels.h",
	""
};


TEST(Meshr, creation, 9)
{
	MeshMemory s;

	FILE* fp;
	if ((fp = fopen("t1.vtk", "r")) == NULL) {
		printf("can't open file\n");
		exit(-1);
	}
	char str[100], meshDescription[256];
	fscanf(fp, "%[^\n]%*c", str);
	fscanf(fp, "%[^\n]%*c", meshDescription); // 
	fscanf(fp, "%[^\n]%*c", str); // ASCII
	if (strcmp(str, "ASCII") != 0) {
		exit(-1);
	}
	fscanf(fp, "%[^\n]%*c", str); //数据信息

	int npoint;
	double* xyz;
	fscanf(fp, "%s", str);  //POINTS
	fscanf(fp, "%d %s\n", &npoint, str);
	if (strcmp(str, "double") == 0) {
		xyz = (double*)malloc(3 * npoint * sizeof(double));
	}
	for (int n = 0; n < npoint; n++) {
		fscanf(fp, "%lf %lf %lf\n", &xyz[3 * n], &xyz[3 * n + 1], &xyz[3 * n + 2]);
	}

	int ncell, cellsize;
	int* cells;
	fscanf(fp, "%s", str);  //CELLS
	fscanf(fp, "%d %d\n", &ncell, &cellsize);
	cells = (int*)malloc(cellsize * sizeof(int));
	int count = 0;
	while (count < cellsize) {
		fscanf(fp, "%d\n", &cells[count]);
		int vex = cells[count];
		count++;
		while (vex) {
			vex--;
			fscanf(fp, "%d\n", &cells[count++]);
		}
	}

	fscanf(fp, "%s", str);  //CELL_TYPES
	fscanf(fp, "%d\n", &ncell);
	int* celltypes = (int*)malloc(ncell * sizeof(int));
	for (int n = 0; n < ncell; n++) {
		fscanf(fp, "%d \n", &celltypes[n]);
	}

	fscanf(fp, "%s", str);  //CELL_DATA
	fscanf(fp, "%d\n", &ncell);
	double* celldata = (double*)malloc(ncell * sizeof(double));
	fscanf(fp, "%[^\n]%*c", str);
	int ncComp = str[strlen(str) - 1] - 48;
	fscanf(fp, "%[^\n]%*c", str);
	for (int n = 0; n < ncell; n++) {
		fscanf(fp, "%lf \n", &celldata[n]);
	}

	fscanf(fp, "%s", str);  //POINT_DATA
	fscanf(fp, "%d\n", &npoint);
	double* pointdata = (double*)malloc(npoint * sizeof(double));
	fscanf(fp, "%[^\n]%*c", str);
	int npComp = str[strlen(str) - 1] - 48;
	fscanf(fp, "%[^\n]%*c", str);
	for (int n = 0; n < npoint; n++) {
		fscanf(fp, "%lf \n", &pointdata[n]);
	}

	s.savetovtkfile("new.vtk", npoint, xyz, ncell, cellsize, 
		cells, celltypes, meshDescription, celldata, pointdata);

	Elements enodes = s.makeElements(npoint, "set_nodes");
	Elements ecells = s.makeElements(ncell, "set_cells");

	Map cell2point = s.makeMap(ecells, enodes, 4, cells, "cell_to_node_map");

	Data dcell1 = s.makeData(ecells, 1, "int", (char*)celltypes, "celltypes");
	Data dcell2 = s.makeData(ecells, 1, "double", (char*)celldata, "data_on_cells");
	Data dpoint1 = s.makeData(enodes, 3, "double", (char*)xyz, "nodes_xyz");
	Data dpoint2 = s.makeData(enodes, 1, "double", (char*)pointdata, "data_on_nodes");

	double sum = 0;
	for (int i = 0; i < ncell; i++) {
		//cout << dcell2->data[i] << endl;
		sum += dcell2->data[i];
	}
	double psum = 0;
	for (int i = 0; i < npoint; i++) {
		//cout << dpoint2->data[i] << endl;
		psum += dpoint2->data[i];
	}
	double psum2 = 0;
	for (int i = 0; i < ncell; i++) {
		psum2 += dpoint2->data[cell2point[i].map[0]] +
			dpoint2->data[cell2point[i].map[1]] +
			dpoint2->data[cell2point[i].map[2]] +
			dpoint2->data[cell2point[i].map[3]];
	}

	CHECK_DOUBLES_EQUAL(sum, 2.662, 0.0000001, 50);
	CHECK_DOUBLES_EQUAL(psum, 76.8, 0.0000001, 50);
	CHECK_DOUBLES_EQUAL(psum2, 167.8, 0.0000001, 50);

}
