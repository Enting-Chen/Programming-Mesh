#include "MeshMemory.h"
#include <iostream>
#include <cstring>
using namespace std;

MeshMemory::MeshMemory() {
}
MeshMemory::~MeshMemory() {
}

Elements MeshMemory::makeElements(int _size, char const* _name) {
	Elements e = (Elements)malloc(sizeof(elements));
	e->size = _size;
	e->name = _name;
	return e;
}
Map MeshMemory::makeMap(Elements f, Elements t, int d1, int* d2, char const* _name) {
	Map m = (map*)malloc(sizeof(map) * f->size);
	m->dim = d1;
	m->from = f;
	m->to = t;
	m->name = _name;
	for (int i = 0; i < f->size; i++) {
		m[i].map = (int*)malloc(sizeof(int) * m->dim);
		for (int j = 0; j < m->dim; j++) {
			m[i].map[j] = d2[(m->dim+1) * i + j + 1];
		}
	}
	return m;
}
Data MeshMemory::makeData(Elements e, int _dim, char const* s, char* _data, char const* _name) {
	Data d = (dat*)malloc(sizeof(dat));
	d->size = e->size;
	d->name = _name;
	d->dim = _dim;
	d->set = e;
	d->data = (double*)malloc(sizeof(double) * d->dim * e->size);
	if (strcmp(s, "int") == 0) {	
		int* data_int = (int*)_data;
		for (int i = 0; i < d->dim * d->size; i++) {
			d->data[i] = data_int[i];
		}
	}
	else if (strcmp(s, "double") == 0) {
		double* data_double = (double*)_data;
		for (int i = 0; i < d->dim * d->size; i++) {
			d->data[i] = data_double[i];
		}	
	}
	return d;
}

bool MeshMemory::savetovtkfile(const char* fileName, int npoint, double* xyz,
	int ncell, int cellsize, int* cells, int* celltypes, const char * meshDescription,
	double* celldata, double* pointdata) {
	printf("writing in grid \n");
	FILE* fp;
	if ((fp = fopen(fileName, "w")) == NULL) {
		printf("can't open file\n");
		return 0;
	}

	//put your code here
	writeSectionLines(fp, meshDescription);
	writeElements(fp, npoint, xyz);
	writeMaps(fp, ncell, cellsize, cells);
	writeData(fp, ncell, npoint, celltypes, celldata, pointdata);

	fclose(fp);
	return true;
}

bool MeshMemory::writeSectionLines(FILE* fp, const char * meshDescription) {
	fprintf(fp, "# vtk DataFile Version %d.%d\n", 2, 0);
	fprintf(fp, "%s\n", meshDescription);
	fprintf(fp, "ASCII\n");
	fprintf(fp, "%s\n", "UNSTRUCTURED_GRID");
	return 1;
}

bool MeshMemory::writeElements(FILE* fp, int npoint, double *xyz) {  
	fprintf(fp, "POINTS %d double\n", npoint);
	for (int n = 0; n < npoint; n++) {
		fprintf(fp, "%lf %lf %lf\n", xyz[3 * n], xyz[3 * n + 1], xyz[3 * n + 2]);
	}  
	return 1;
}

bool MeshMemory::writeMaps(FILE* fp, int ncell, int cellsize, int * cells) {  
	fprintf(fp, "\nCELLS %d %d\n", ncell, cellsize);
	int count = 0;
	while (count < cellsize) {
		fprintf(fp, "%d ", cells[count]);
		int vex = cells[count];
		count++;
		while (vex) {
			vex--;
			fprintf(fp, "%d ", cells[count++]);
		}
		fprintf(fp, "\n");
	}
	return 1;
}
bool MeshMemory::writeData(FILE* fp, int ncell, int npoint, int * celltypes, double * celldata, double * pointdata) { 
	fprintf(fp, "\nCELL_TYPES %d \n", ncell);
	for (int n = 0; n < ncell; n++) {
		fprintf(fp, "%d \n", celltypes[n]);
	}
	fprintf(fp, "\nCELL_DATA %d \nSCALARS CellEntityIds double 1 \nLOOKUP_TABLE default\n", ncell);
	for (int n = 0; n < ncell; n++) {
		fprintf(fp, "%lf \n", celldata[n]);
	}
	fprintf(fp, "\nPOINT_DATA %d \nSCALARS CellEntityIds double 1 \nLOOKUP_TABLE default\n", npoint);
	for (int n = 0; n < npoint; n++) {
		fprintf(fp, "%lf \n", pointdata[n]);
	}

	return 1;
}