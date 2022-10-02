#include "MeshVTK.h"

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
    readSectionLines( fp );
    readPoints( fp );
    readCells( fp );
    readCellType( fp );
    readData( fp );

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

    //put your code here
    writeSectionLines( fp );
    writePoints( fp );
    writeCells( fp );
    writeCellType( fp );
    writeData( fp );

    fclose( fp );
    return true;
}

void MeshVTK::init() {
}

bool MeshVTK::readSectionLines( FILE* fp ) {
    fgets( s1, 50, fp );
    fgets( s2, 50, fp );
    fgets( s3, 50, fp );
    fgets( s4, 50, fp );
    return 1;
}
bool MeshVTK::readPoints( FILE* fp ) {
    char s[ 50 ];
    fscanf( fp, "%s", s );
    fscanf( fp, "%d", &npoint );
    fscanf( fp, "%s", s );
    //printf( "%s\n", s1 );
    //printf( "%d\n", npoint );
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
    char s[ 50 ];
    printf( "reading cell\n" );
    fscanf( fp, "%s %d %d\n", s, &ncell, &cellsize );
    //printf("%s\n", ncell);
    //printf( "%d\n", ncell );
    cells = (int*)malloc( cellsize * sizeof( int ) );
    for ( int n = 0; n < cellsize; n++ ) {
        if ( fscanf( fp, "%d\n", &cells[ n ] ) != 1 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
        //printf( "%d\n", cells[ n ] );
    }
    return 1;
}
bool MeshVTK::readCellType( FILE* fp ) {
    char s[ 50 ] = "\n";
    fgets( s, 50, fp );
    //fgets(s, 50, fp);
    printf( "reading celltype\n" );
    //printf( "%d\n", cellsize );
    celltype = (int*)malloc( ncell * sizeof( int ) );
    for ( int n = 0; n < ncell; n++ ) {
        if ( fscanf( fp, "%d\n", &celltype[ n ] ) != 1 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
    }
    return 1;
}

bool MeshVTK::writeSectionLines( FILE* fp ) {
    fprintf( fp, "%s", s1 );
    fprintf( fp, "%s", s2 );
    fprintf( fp, "%s", s3 );
    fprintf( fp, "%s", s4 );
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
bool MeshVTK::writeCells( FILE* fp ) {
    fprintf( fp, "CELLS %d %d\n", ncell, cellsize );
    int i = 0;
    for ( int n = 0; n < ncell; n++ ) {
        switch (celltype[n])
        {
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

bool MeshVTK::writeCellType( FILE* fp ) {
    fprintf( fp, "CELL_TYPES %d\n", ncell );
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d\n", celltype[ n ] );
    }
    fprintf( fp, "\n" );
    return 1;
}

bool MeshVTK::readData( FILE* fp ) {
    char s[ 50 ] = "\n";
    fgets( s, 50, fp );
    fgets( s, 50, fp );
    fgets( s, 50, fp );
    //fgets( s, 50, fp );
    printf( "read data\n" );
    //printf( "%s", s );
    q = (int*)malloc( ncell * sizeof( int ) );
    for ( int n = 0; n < ncell; n++ ) {
        if ( fscanf( fp, "%d\n", &q[ n ] ) != 1 ) {
            printf( "error reading\n" );
            exit( -1 );
        }
    }
    return 1;
}

bool MeshVTK::writeData( FILE* fp ) {
    fprintf( fp, "CELL_DATA %d\n", ncell );
    fprintf( fp, "SCALARS CellEntityIds int 1\n" );
    fprintf( fp, "LOOKUP_TABLE default\n" );
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d\n", q[ n ] );
    }
    return 1;
}
