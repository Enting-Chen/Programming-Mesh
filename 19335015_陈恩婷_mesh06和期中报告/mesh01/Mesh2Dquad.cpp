#include "Mesh2Dquad.h"

Mesh2Dquad::Mesh2Dquad() {
}
Mesh2Dquad::~Mesh2Dquad() {
	delete[] becell;
	delete[] bound;
	delete[] bedge;
	delete[] edge;
	delete[] cell;
	delete[] cell2edge;
	//becell, * ecell, * bound, * bedge, * edge, * cell, * cell2edge;
}
bool Mesh2Dquad::read(const char* fileName) {
	printf("reading in grid \n");

	FILE* fp;
	if ((fp = fopen(fileName, "r")) == NULL) {
		printf("can't open file\n"); 
		return 0;
	}
	readHeader(fp);
	init();
	readNode(fp); 
	readCell(fp);
	readEdge(fp);
	readBEdge(fp);

	fclose(fp);
	return true;
}

bool Mesh2Dquad::save(const char* fileName) {
	printf("writing in grid \n");
	FILE* fp;
	if ((fp = fopen(fileName, "w")) == NULL) {
		printf("can't open file\n");
		return 0;
	}
	writeHeader(fp);
	writeNode(fp);
	writeCell(fp);
	writeEdge(fp);
	writeBEdge(fp);

	fclose(fp);
	return true;
}

void Mesh2Dquad::init() {
}


bool Mesh2Dquad::readHeader(FILE* fp) {
    if ( fscanf( fp, "%d %d %d %d \n", &nnode, &ncell, &nedge, &nbedge ) != 4 ) {
        printf( "error reading from new_grid.dat\n" );
        exit( -1 );
    }
    cell = (int*)malloc( 4 * ncell * sizeof( int ) );
    edge = (int*)malloc( 2 * nedge * sizeof( int ) );
    ecell = (int*)malloc( 2 * nedge * sizeof( int ) );
    bedge = (int*)malloc( 2 * nbedge * sizeof( int ) );
    becell = (int*)malloc( nbedge * sizeof( int ) );
    bound = (int*)malloc( nbedge * sizeof( int ) );

    x = (real*)malloc( 2 * nnode * sizeof( real ) );
    cell2edge = (int*)malloc( 4 * ncell * sizeof( int ) );
    for ( int i = 0; i < 4 * ncell; ++i ) {
        cell2edge[ i ] = -1;
    }
    return 1;
}
bool Mesh2Dquad::readNode(FILE* fp) {
    for ( int n = 0; n < nnode; n++ ) {
        if ( fscanf( fp, "%lf %lf \n", &x[ 2 * n ], &x[ 2 * n + 1 ] ) != 2 ) {
            printf( "error reading from new_grid.dat\n" );
            exit( -1 );
        }
    }
    return 1;
}
bool Mesh2Dquad::readCell(FILE* fp) {
    for ( int n = 0; n < ncell; n++ ) {
        if ( fscanf( fp, "%d %d %d %d \n", &cell[ 4 * n ], &cell[ 4 * n + 1 ],
                     &cell[ 4 * n + 2 ], &cell[ 4 * n + 3 ] ) != 4 ) {
            printf( "error reading from new_grid.dat\n" );
            exit( -1 );
        }
    }
    return 1;
}
bool Mesh2Dquad::readEdge(FILE* fp) {
    for ( int n = 0; n < nedge; n++ ) {
        if ( fscanf( fp, "%d %d %d %d \n", &edge[ 2 * n ], &edge[ 2 * n + 1 ],
                     &ecell[ 2 * n ], &ecell[ 2 * n + 1 ] ) != 4 ) {
            printf( "error reading from new_grid.dat\n" );
            exit( -1 );
        }
        for ( int i = 0; i < 4; ++i ) {
            if ( cell2edge[ ecell[ 2 * n ] + i ] == -1 ) {
                cell2edge[ ecell[ 2 * n ] + i ] = n;
                break;
            }
        }
        for ( int i = 0; i < 4; ++i ) {
            if ( cell2edge[ ecell[ 2 * n + 1 ] + i ] == -1 ) {
                cell2edge[ ecell[ 2 * n + 1 ] + i ] = n;
                break;
            }
        }
    }
    return 1;
}
bool Mesh2Dquad::readBEdge(FILE* fp) {
    for ( int n = 0; n < nbedge; n++ ) {
        if ( fscanf( fp, "%d %d %d %d \n", &bedge[ 2 * n ], &bedge[ 2 * n + 1 ],
                     &becell[ n ], &bound[ n ] ) != 4 ) {
            printf( "error reading from new_grid.dat\n" );
            exit( -1 );
        }
    }
    return 1;
}

bool Mesh2Dquad::writeHeader(FILE* fp) {
    fprintf( fp, "%d %d %d %d \n", nnode, ncell, nedge, nbedge );
    return 1;
}
bool Mesh2Dquad::writeNode(FILE* fp) {
    for ( int n = 0; n < nnode; n++ ) {
        fscanf(fp, "%lf %lf \n", x[2 * n], x[2 * n + 1]);
    }
    return 1;
}
bool Mesh2Dquad::writeCell(FILE* fp) {
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d %d %d %d \n", cell[ 4 * n ], cell[ 4 * n + 1 ],
                     cell[ 4 * n + 2 ], cell[ 4 * n + 3 ] );
    }
    return 1;
}
bool Mesh2Dquad::writeEdge(FILE* fp) {
    for ( int n = 0; n < nedge; n++ ) {
        fprintf( fp, "%d %d %d %d \n", edge[ 2 * n ], edge[ 2 * n + 1 ],
                     ecell[ 2 * n ], cell[ 2 * n + 1 ] );
    }
    return 1;
}
bool Mesh2Dquad::writeBEdge(FILE* fp) {
    for ( int n = 0; n < nbedge; n++ ) {
        fprintf(fp, "%d %d %d %d \n", bedge[2 * n], bedge[2 * n + 1],
            becell[n], bound[n]);
    }
    return 1;
}
