#include "Executable.h"     //Comment this line if your code could run with error and you want to submit on line
#ifdef CodeExecutable

#ifndef MeshMemory_h
#define MeshMemory_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef double real;
typedef struct {
	int index;        /* index */
	int size;         /* number of elements in set */
	char const name[30]; /* name of set */
}  elements;

typedef  elements* Elements;

typedef struct {
	int index;        /* index */
	Elements from,      /* set pointed from */
		to;           /* set pointed to */
	int dim,          /* dimension of pointer */
		* map;         /* array defining pointer */
	char const name[30]; /* name of pointer */
} map;

typedef map* Map;

typedef struct {
	int index;        /* index */
	Elements set;       /* set on which data is defined */
	int dim,          /* dimension of data */
		size;         /* size of each element in dataset */
	char* data;       /* data */
	char const type[16], /* datatype */
		name[30];        /* name of dataset */
} dat;

typedef dat* Data;

class Mesh
{

public:
	int element_list_size, map_list_size, dat_list_size,
		element_list_index, map_list_index, dat_list_index;
	Elements* element_list;
	Map* map_list;
	Data* dat_list;

public:
	Elements makeElements(int, char const*);
	Map makeMap(Elements, Elements, int, int*, char const*);
	Data makeData(Elements, int, char const*, char*, char const*);

	Mesh();
	Mesh(char* filename) {};
	~Mesh() {};

	void init() {};
	//emd file
	bool WriteMeshToEMDAscii(const char* fileName);
	bool ReadMeshFromEMDAscii(const char* fileName);  //allow reload from a file

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
};

	//read raw file

	bool readrawHeader(FILE* fp, int *nnode, int *ncell, int*nedge, int*nbedge);
	bool readrawNode(FILE* fp, int nnode, double * xyz);
	bool readrawCell(FILE* fp, int ncell, int*cells);
	bool readrawEdge(FILE* fp, int nedge, int*edge, int*ecell);
	bool readrawBEdge(FILE* fp, int nbedge, int*bedge, int*becell, int*bound);
	bool readrawQ(FILE* fp, int ncell, double* q);

	bool writerawHeader(FILE* fp, int nnode, int ncell, int nedge, int nbedge);
	bool writerawNode(FILE* fp, int nnode, double* xyz);
	bool writerawCell(FILE* fp, int ncell, int* cell);
	bool writerawEdge(FILE* fp, int nedge, int* edge, int* ecell);
	bool writerawBEdge(FILE* fp, int nbedge, int* bedge, int* becell, int* bound);
	bool writerawQ(FILE* fp, int ncell, double* q);

	void WriteRaw(const char* fileName, int nnode, int ncell, int nedge, int nbedge, double* x, int* cell, 
		int* edge, int* ecell, int* bedge, int* becell, int* bound, double* q);
	void ReadRaw(const char* fileName, int* nnode, int* ncell, int* nedge, int* nbedge, double** x, int** cell, 
		int** edge, int** ecell, int** bedge, int** becell, int** bound, double** q);
	void WriteMeshToVTKAscii(const char* filename, double* nodeCoords_data, int nnode, int* cellsToNodes_data, 
		int ncell, double* values_data);

#endif
#endif // CodeExecutable

