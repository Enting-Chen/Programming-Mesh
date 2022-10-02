#include "Executable.h"     //Comment this line if your code could run with error
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
	char* data;       /* data */
	char const* type, /* datatype */
		* name;        /* name of dataset */
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
	MeshMemory(char * filename);
	~MeshMemory();

	bool savetofile(const char* fileName);
	bool readfromfile(const char* fileName);

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

};

#endif
#endif // CodeExecutable
