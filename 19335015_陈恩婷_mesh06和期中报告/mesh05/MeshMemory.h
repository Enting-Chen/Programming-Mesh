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
	char const* name; /* name of set */
}  elements;

typedef  elements* Elements;

typedef struct {
	int index;        /* index */
	Elements from,      /* set pointed from */
		to;           /* set pointed to */
	int dim,          /* dimension of pointer */
		* map;         /* array defining pointer */
	char const* name; /* name of pointer */
} map;

typedef map* Map;

typedef struct {
	int index;        /* index */
	Elements set;       /* set on which data is defined */
	int dim,          /* dimension of data */
		size;         /* size of each element in dataset */
	double* data;       /* data */
	char const* name;        /* name of dataset */
} dat;

typedef dat* Data;


class MeshMemory
{

public:
	Elements* element_list;
	Map* map_list;
	Data* dat_list;

	Elements makeElements(int, char const*);
	Map makeMap(Elements, Elements, int, int*, char const*);
	Data makeData(Elements, int, char const*, char*, char const*);

public:
	MeshMemory();
	~MeshMemory();

	bool savetovtkfile(const char* fileName, int npoint, double* xyz,
		int ncell, int cellsize, int* cells, int* celltypes, const char * meshDescription,
		double* celldata, double* pointdata);

	//以下声明可以改变,自己重新定义函数原型
//write functions
	bool writeSectionLines(FILE* fp, const char * meshDescription);
	bool writeElements(FILE* fp, int npoint, double* xyz);
	bool writeMaps(FILE* fp, int ncell, int cellsize, int* cells);
	bool writeData(FILE* fp, int ncell, int npoint, int* celltypes, double* celldata, double* pointdata);

};

#endif
