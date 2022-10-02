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
	char name[300]; /* name of set */
}  elements;

typedef  elements* Elements;

typedef struct {
	int index;        /* index */
	Elements from,      /* set pointed from */
		to;           /* set pointed to */
	int dim,          /* dimension of pointer */
		* map;         /* array defining pointer */
	char name[300]; /* name of pointer */
} map;

typedef map* Map;

typedef struct {
	int index;        /* index */
	Elements set;       /* set on which data is defined */
	int dim,          /* dimension of data */
		size;         /* size of each element in dataset */
	char* data;       /* data */
	char type[300], /* datatype */
		name[300];        /* name of dataset */
} dat;

typedef dat* Data;


class MeshMemory
{

public:
	int element_list_size, map_list_size, dat_list_size,
		element_list_index, map_list_index, dat_list_index;
	Elements* element_list;
	Map* map_list;
	Data* dat_list;

	Elements makeElements(int, char const*);
	Map makeMap(Elements, Elements, int, int*, char const*);
	Data makeData(Elements, int, char const*, char*, char const*);

public:
	MeshMemory();
	MeshMemory(char* filename);
	~MeshMemory();

	void init();
	bool savetofile(const char* fileName);
	bool readfromfile(const char* fileName);  //allow reload from a file

	//read functions
	bool readHeader(FILE* fp);
	bool readElements(FILE* fp);
	bool readMaps(FILE* fp);
	bool readData(FILE* fp);

	//write functions
	bool writeHeader(FILE* fp);
	bool writeElements(FILE* fp);
	bool writeMaps(FILE* fp);
	bool writeData(FILE* fp);

	//read from vtk ASCII file
	bool readvtkHeader(FILE* fp,char* meshDes);  //return mesh Description
	bool readvtkPoints(FILE* fp,int& npoint,double*& xyz);  //return number of points
	bool readvtkCells(FILE* fp, int& ncell,int& size, int*& cells);   //return number of cells
	bool readvtkCellTypes(FILE* fp,int ncell,int*& celltypes);
	bool readvtkPointData(FILE* fp,int npoint, int& type, int& dim, double*& pointdata);
	bool readvtkCellData(FILE* fp,int ncell, int& type, int& dim,double*& celldata);

};

#endif
#endif // CodeExecutable

