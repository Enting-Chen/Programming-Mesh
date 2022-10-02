#ifndef MeshVTK_h
#define MeshVTK_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

class MeshVTK
{

public:
	int vtkVer, vtkSubVer;
	char vtkDescription[256];  //Dataset description
	bool ascii;
	char dataset[32];	//Dataset name
	int  npoint,		//number of points
		ncell,			//number of cells
		cellsize;		//the size of cells info
	double* xyz;		//point xyz, only double is considered
	int* cells, *celltypes; //cells info, each cell type
	
	//only type int (1 for each cell) is considered. 
	int* pointdata, psize, npComp;
	int* celldata, csize, ncComp;

	//LOOKUP_TABLE default

public:
	MeshVTK();
	~MeshVTK();

	bool read(const char* fileName);
	bool save(const char* fileName);

	//以下声明可以改变,自己重新定义函数原型
public://private:
	//void init();
	//bool readSectionLines(FILE* fp);
	bool readPoints(FILE* fp);
	bool readCells(FILE* fp);
	bool readCellType(FILE* fp);
	bool readCellData(FILE* fp);
	bool readPointData(FILE* fp);

//write functions
	bool writePoints(FILE* fp);
	bool writeCells(FILE* fp);
	bool writeCellType(FILE* fp);
	bool writeCellData(FILE* fp);
	bool writePointData(FILE* fp);

};

#endif
