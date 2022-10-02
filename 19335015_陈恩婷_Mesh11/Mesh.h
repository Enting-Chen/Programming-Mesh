#include "Executable.h"     //Comment this line if your code could run with error and you want to submit on line
#ifdef CodeExecutable

#ifndef MeshMemory_h
#define MeshMemory_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

typedef double real;
typedef struct {
	int index;        /* index */
	int size;         /* number of elements in set */
	char const name[16]; /* name of set */
}  elements;

typedef  elements* Elements;

typedef struct {
	int index;        /* index */
	Elements from,      /* set pointed from */
		to;           /* set pointed to */
	int dim,          /* dimension of pointer */
		* map;         /* array defining pointer */
	char const name[16]; /* name of pointer */
} map;

typedef map* Map;

typedef struct {
	int index;        /* index */
	Elements set;       /* set on which data is defined */
	int dim,          /* dimension of data */
		size;         /* size of each element in dataset */
	char* data;       /* data */
	char const type[16], /* datatype */
		name[16];        /* name of dataset */
} dat;

typedef dat* Data;


	Elements makeElements(int, char const*);
	Map makeMap(Elements, Elements, int, int*, char const*);
	Data makeData(Elements, int, char const*, char*, char const*);

	void init(); 
	bool readfromfileuser(const char* fileName);  //allow reload from a file

	//emd file
	bool savetofile(const char* fileName);
	bool readfromfile(const char* fileName);  //allow reload from a file

	//read emd functions
	bool readHeader(FILE* fp);
	bool readElements(FILE* fp);
	bool readMaps(FILE* fp);
	bool readData(FILE* fp);

	//write emd functions
	bool writeHeader(FILE* fp);
	bool writeElements(FILE* fp);
	bool writeMaps(FILE* fp);
	bool writeData(FILE* fp);

	//read from vtk ASCII file
	bool readvtk(const char* fileName);

	bool readvtkHeader(FILE* fp, char* meshDes);  //return mesh Description
	bool readvtkPoints(FILE* fp, int& npoint, double*& xyz);  //return number of points
	bool readvtkCells(FILE* fp, int& ncell, int& size, int*& cells);   //return number of cells
	bool readvtkCellTypes(FILE* fp, int ncell, int*& celltypes);
	bool readvtkPointData(FILE* fp, int npoint, int& type, int& dim, char*& pointdata);
	bool readvtkCellData(FILE* fp, int ncell, int& type, int& dim, char*& celldata);

	//write to vtk file
	bool savevtk(const char* fileName);

	bool writevtkHeader(FILE* fp);
	bool writevtkPoints(FILE* fp);
	bool writevtkCells(FILE* fp);
	bool writevtkCellType(FILE* fp);
	bool writevtkCellData(FILE* fp);
	bool writevtkPointData(FILE* fp);

	//read raw file
	bool readraw(const char* fileName);

	bool readrawHeader(FILE* fp, int *nnode, int *ncell, int*nedge, int*nbedge);
	bool readrawNode(FILE* fp, int nnode, double * xyz);
	bool readrawCell(FILE* fp, int ncell, int*cells);
	bool readrawEdge(FILE* fp, int nedge, int*edge, int*ecell);
	bool readrawBEdge(FILE* fp, int nbedge, int*bedge, int*becell, int*bound);

	void WriteMeshToVTKAscii(const char* filename, double* nodeCoords_data, int nnode, int* cellsToNodes_data, int ncell, double* values_data);


#endif
#endif // CodeExecutable

