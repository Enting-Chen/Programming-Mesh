#include "MeshVTK3.h"

#include <cstring>
#include <iostream>
using namespace std;

MeshVTK::MeshVTK() {
}
MeshVTK::~MeshVTK() {
}
bool MeshVTK::read( const char* fileName ) {
    printf( "reading in grid \n" );

    FILE* fp;
    if ( ( fp = fopen( fileName, "r" ) ) == NULL ) {
        printf( "can't open file\n" );
        return 0;
    }
    //put your code here
    char s[ 100 ];
    fscanf( fp, "# vtk DataFile Version %d.%d\n", &vtkVer, &vtkSubVer );
    fscanf( fp, "%[^, ], Created by Gmsh\n%s\n", &dataset, &s );
    if (strcmp(s, "ASCII") != 0) {
        cout << "Not text file, exit" << endl;
        exit(-1);
    }
    fscanf(fp, "DATASET %s\n", &s);
    if (strcmp(s, "UNSTRUCTURED_GRID") != 0) {
        cout << "Not UNSTRUCTURED_GRID, exit" << endl;
        exit(-1);
    }
    while ( fscanf( fp, "%s", s ) != EOF ) {
        if ( strcmp( s, "POINTS" ) == 0 ) {
            readPoints( fp );
        }
        else if ( strcmp( s, "CELLS" ) == 0 ) {
            readCells( fp );
        }
        else if ( strcmp( s, "CELL_TYPES" ) == 0 ) {
            readCellType( fp );
        }
        else if ( strcmp( s, "CELL_DATA" ) == 0 ) {
            readCellData( fp );
        }
    }

    fclose( fp );
    return true;
}

bool MeshVTK::save( const char* fileName ) {
    printf( "writing in grid \n" );
    FILE* fp;
    if ( ( fp = fopen( fileName, "w" ) ) == NULL ) {
        printf( "can't open file\n" );
        return 0;
    }
    fprintf( fp, "# vtk DataFile Version %d.%d\n", vtkVer, vtkSubVer );
    fprintf( fp, "%s, Created by Gmsh\nASCII\nDATASET UNSTRUCTURED_GRID\n", dataset );
    writePoints( fp );
    writeCells( fp );

    writeCellType( fp );
    writeCellData( fp );
    fclose( fp );

    return true;
}

bool MeshVTK::readPoints( FILE* fp ) {
    fscanf( fp, "%d double", &npoint );
    xyz = (double*)malloc( 3 * npoint * sizeof( double ) );
    for ( int n = 0; n < npoint; n++ ) {
        if ( fscanf( fp, "%lf %lf %lf\n", &xyz[ 3 * n ], &xyz[ 3 * n + 1 ], &xyz[ 3 * n + 2 ] ) != 3 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
    }
    return 1;
}
bool MeshVTK::readCells( FILE* fp ) {
    fscanf( fp, "%d %d\n", &ncell, &cellsize );
    cells = (int*)malloc( cellsize * sizeof( int ) );
    for ( int n = 0; n < cellsize; n++ ) {
        if ( fscanf( fp, "%d\n", &cells[ n ] ) != 1 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
    }
    return 1;
}
bool MeshVTK::readCellType( FILE* fp ) {
    fscanf( fp, "%d\n", &ncell );
    celltypes = (int*)malloc( ncell * sizeof( int ) );
    for ( int n = 0; n < ncell; n++ ) {
        if ( fscanf( fp, "%d\n", &celltypes[ n ] ) != 1 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
    }
    return 1;
}
bool MeshVTK::readCellData( FILE* fp ) {
    char s[ 50 ] = "\n";
    fscanf( fp, "%d\n", &csize );
    fgets( s, 50, fp );
    fgets( s, 50, fp );
    celldata = (int*)malloc( csize * sizeof( int ) );
    for ( int n = 0; n < csize; n++ ) {
        if ( fscanf( fp, "%d\n", &celldata[ n ] ) != 1 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
    }
    return 1;
}
bool MeshVTK::readPointData( FILE* fp ) {
    return 1;
}

bool MeshVTK::writeCellData( FILE* fp ) {
    fprintf( fp, "CELL_DATA %d\n", ncell );
    fprintf( fp, "SCALARS CellEntityIds int 1\n" );
    fprintf( fp, "LOOKUP_TABLE default\n" );
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d\n", celldata[ n ] );
    }
    return 1;
}

bool MeshVTK::writeCellType( FILE* fp ) {
    fprintf( fp, "CELL_TYPES %d\n", ncell );
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d\n", celltypes[ n ] );
    }
    fprintf( fp, "\n" );
    return 1;
}
bool MeshVTK::writeCells( FILE* fp ) {
    fprintf( fp, "CELLS %d %d\n", ncell, cellsize );
    int i = 0;
    for ( int n = 0; n < ncell; n++ ) {
        switch ( celltypes[ n ] ) {
        case 1:
            fprintf( fp, "%d %d\n", cells[ i ], cells[ i + 1 ] );
            i += 2;
            break;
        case 3:
            fprintf( fp, "%d %d %d\n", cells[ i ], cells[ i + 1 ], cells[ i + 2 ] );
            i += 3;
            break;
        case 5:
            fprintf( fp, "%d %d %d %d\n", cells[ i ], cells[ i + 1 ], cells[ i + 2 ], cells[ i + 3 ] );
            i += 4;
            break;
        default:
            break;
        }
    }
    fprintf( fp, "\n" );
    return 1;
}
bool MeshVTK::writePoints( FILE* fp ) {
    fprintf( fp, "POINTS %d double\n", npoint );
    for ( int n = 0; n < npoint; n++ ) {
        fprintf( fp, "%lf %lf %lf\n", xyz[ 3 * n ], xyz[ 3 * n + 1 ], xyz[ 3 * n + 2 ] );
    }
    fprintf( fp, "\n" );
    return 1;
}
bool MeshVTK::writePointData( FILE* fp ) {
    return 1;
}
