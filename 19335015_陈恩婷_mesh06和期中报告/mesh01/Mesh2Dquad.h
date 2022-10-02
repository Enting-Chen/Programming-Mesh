#ifndef Mesh2Dquad_h
#define Mesh2Dquad_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
typedef double real;


class Mesh2Dquad
{

public:
	int  nnode, ncell, nedge, nbedge;
	int* becell, * ecell, * bound, * bedge, * edge, * cell, *cell2edge;
	real* x;
public:
	Mesh2Dquad();
	~Mesh2Dquad();

	bool read(const char* fileName);
	bool save(const char* fileName);

public://private
	void init();
	bool readHeader(FILE* fp);
	bool readNode(FILE* fp);
	bool readCell(FILE* fp);
	bool readEdge(FILE* fp);
	bool readBEdge(FILE* fp);

	bool writeHeader(FILE* fp);
	bool writeNode(FILE* fp);
	bool writeCell(FILE* fp);
	bool writeEdge(FILE* fp);
	bool writeBEdge(FILE* fp);

};

#endif
