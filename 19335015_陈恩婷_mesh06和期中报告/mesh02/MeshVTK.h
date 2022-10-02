#ifndef Mesh2Dquad_h
#define Mesh2Dquad_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

class MeshVTK
{

public:
	int  npoint, ncell,cellsize;
	int* cells, *celltype;
	double* xyz;
	int * q;
    char s1[ 30 ];
    char s2[ 30 ];
    char s3[ 30 ];
    char s4[ 30 ];

   public:
    MeshVTK();
    ~MeshVTK();

    bool read( const char* fileName );
    bool save( const char* fileName );

   public:  //private:
    void init();
    bool readSectionLines( FILE* fp );
    bool readPoints( FILE* fp );
    bool readCells( FILE* fp );
    bool readCellType( FILE* fp );
    bool readData( FILE* fp );
    bool writeSectionLines( FILE* fp );
    bool writePoints( FILE* fp );
    bool writeCells( FILE* fp );
    bool writeCellType( FILE* fp );

    //write functions
    bool writeData( FILE* fp );

};

#endif
