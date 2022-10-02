#include "Mesh.h"
#ifdef CodeExecutable
#include <cstring>
#include <iostream>
using namespace std;
Mesh::Mesh() {
    element_list_index = 0;
    map_list_index = 0;
    dat_list_index = 0;
    element_list_size = 10;
    map_list_size = 10;
    dat_list_size = 10;
    element_list = (Elements*)malloc( sizeof( Elements ) * element_list_size );
    map_list = (Map*)malloc( sizeof( Map ) * map_list_size );
    dat_list = (Data*)malloc( sizeof( Data ) * dat_list_size );
    memset( element_list, 0, sizeof( Map ) * map_list_size );
    memset( map_list, 0, sizeof( Elements ) * element_list_size );
    memset( dat_list, 0, sizeof( Data ) * dat_list_size );
}

Mesh::Mesh( char* fname ) {
    element_list_index = 0;
    map_list_index = 0;
    dat_list_index = 0;
    element_list_size = 10;
    map_list_size = 10;
    dat_list_size = 10;
    element_list = (Elements*)malloc( sizeof( Elements ) * element_list_size );
    map_list = (Map*)malloc( sizeof( Map ) * map_list_size );
    dat_list = (Data*)malloc( sizeof( Data ) * dat_list_size );
    memset( element_list, 0, sizeof( Map ) * map_list_size );
    memset( map_list, 0, sizeof( Elements ) * element_list_size );
    memset( dat_list, 0, sizeof( Data ) * dat_list_size );
    readfromfile( fname );
}

Mesh::~Mesh() {
    // to do sth
    free( element_list );
    free( map_list );
    free( dat_list );
}

////////////////////////////////////////////////
void Mesh::init() {
    // to do sth}
    memset( element_list, 0, sizeof( Map ) * map_list_size );
    memset( map_list, 0, sizeof( Elements ) * element_list_size );
    memset( dat_list, 0, sizeof( Data ) * dat_list_size );
    element_list_index = 0;
    map_list_index = 0;
    dat_list_index = 0;
}
Elements Mesh::makeElements( int ele_size, char const* ele_name ) {
    Elements ele = (Elements)malloc( sizeof( elements ) );
    strcpy( ele->name, ele_name );
    ele->size = ele_size;
    ele->index = element_list_index;
    element_list[ element_list_index++ ] = ele;
    return ele;
}
Map Mesh::makeMap( Elements map_from, Elements map_to, int map_dim, int* map_map, char const* map_name ) {
    int arr_cnt = map_from->size;
    Map mapi = (Map)malloc( sizeof( map ) );
    mapi->index = map_list_index;
    mapi->from = map_from;
    mapi->to = map_to;
    mapi->dim = map_dim;
    mapi->map = map_map;
    strcpy( (char*)( mapi->name ), map_name );
    map_list[ map_list_index++ ] = (Map)mapi;
    return (Map)mapi;
}
Data Mesh::makeData( Elements data_set, int data_dim, char const* data_type, char* data_data, char const* data_name ) {
    Data data = (Data)malloc( sizeof( dat ) );
    data->index = dat_list_index;
    data->set = data_set;
    data->dim = data_dim;
    data->size = 8;
    data->data = data_data;
    strcpy( (char*)( data->name ), data_name );
    strcpy( (char*)( data->type ), data_type );
    dat_list[ dat_list_index++ ] = data;
    return data;
}

/////////////////////////////////////////////////

bool Mesh::readfromfile( const char* fileName ) {  /////////////////////////////
    FILE* fp;
    if ( ( fp = fopen( fileName, "rb" ) ) == NULL ) {
        printf( "can't open file\n" );
        return 0;
    }

    init();
    readHeader( fp );
    readElements( fp );
    readMaps( fp );
    readData( fp );

    fclose( fp );
    return true;
}

bool Mesh::savetofile( const char* fileName ) {  //////////////////////////
    ( "writing in grid \n" );
    FILE* fp;
    if ( ( fp = fopen( fileName, "wb" ) ) == NULL ) {
        printf( "can't open file\n" );
        return 0;
    }

    writeHeader( fp );
    writeElements( fp );
    writeMaps( fp );
    writeData( fp );

    fclose( fp );
    return true;
}

/////////////////////////////////////// vtk /////////////////////////
bool Mesh::readvtkHeader( FILE* fp, char* meshDescription ) {
    char str[ 100 ];
    fscanf( fp, "%[^\n]%*c", str );
    fscanf( fp, "%[^\n]%*c", meshDescription );  //
    fscanf( fp, "%[^\n]%*c", str );              // ASCII
    if ( strcmp( str, "ASCII" ) != 0 ) {
        exit( -1 );
    }
    fscanf( fp, "%[^\n]%*c", str );  //数据信息

    return 0;
}

bool Mesh::readvtkPoints( FILE* fp, int& npoint, double*& xyz ) {
    char str[ 100 ];
    fscanf( fp, "%s", str );  // POINTS
    fscanf( fp, "%d %s\n", &npoint, str );
    if ( strcmp( str, "double" ) == 0 ) {
        xyz = (double*)malloc( 3 * npoint * sizeof( double ) );
    }
    for ( int n = 0; n < npoint; n++ ) {
        fscanf( fp, "%lf %lf %lf\n", &( xyz[ 3 * n ] ), &( xyz[ 3 * n + 1 ] ), &( xyz[ 3 * n + 2 ] ) );
    }
    return 0;
}

bool Mesh::readvtkCells( FILE* fp, int& ncell, int& cellsize, int*& cells ) {
    char str[ 100 ];
    fscanf( fp, "%s", str );  // CELLS
    fscanf( fp, "%d %d\n", &ncell, &cellsize );
    cells = (int*)malloc( cellsize * sizeof( int ) );
    int count = 0, count1 = 0, tt;
    while ( count < cellsize ) {
        fscanf( fp, "%d\n", &tt );
        int vex = tt;
        count++;
        while ( vex-- ) {
            fscanf( fp, "%d\n", &cells[ count1++ ] );
            count++;
        }
    }
    return 0;
}

bool Mesh::readvtkCellTypes( FILE* fp, int ncell, int*& celltypes ) {
    char str[ 100 ];
    fscanf( fp, "%s", str );  // CELL_TYPES
    fscanf( fp, "%d\n", &ncell );
    celltypes = (int*)malloc( ncell * sizeof( int ) );
    for ( int n = 0; n < ncell; n++ ) {
        fscanf( fp, "%d \n", &celltypes[ n ] );
    }
    return 0;
}

bool Mesh::readvtkPointData( FILE* fp, int npoint, int& type, int& dim, double*& pointdata ) {
    char str[ 100 ];
    fscanf( fp, "%s", str );  // POINT_DATA
    fscanf( fp, "%d\n", &npoint );
    pointdata = (double*)malloc( npoint * sizeof( double ) );  // npoint
    fscanf( fp, "%[^\n]%*c", str );
    int npComp = str[ strlen( str ) - 1 ] - 48;
    fscanf( fp, "%[^\n]%*c", str );
    for ( int n = 0; n < npoint; n++ ) {
        fscanf( fp, "%lf \n", &pointdata[ n ] );
    }
    return 0;
}

bool Mesh::readvtkCellData( FILE* fp, int ncell, int& type, int& dim, double*& celldata ) {
    char str[ 100 ];
    fscanf( fp, "%s", str );  // CELL_DATA
    fscanf( fp, "%d\n", &ncell );
    celldata = (double*)malloc( ncell * sizeof( double ) );
    fscanf( fp, "%[^\n]%*c", str );
    int ncComp = str[ strlen( str ) - 1 ] - 48;
    fscanf( fp, "%[^\n]%*c", str );
    for ( int n = 0; n < ncell; n++ ) {
        fscanf( fp, "%lf \n", &celldata[ n ] );
    }
    return 0;
}

bool Mesh::writeHeader( FILE* fp ) {
    fprintf( fp, "#emd %d %d %d %d %d %d\n", element_list_size, map_list_size, dat_list_size,
             element_list_index, map_list_index, dat_list_index );
    return 1;
}

bool Mesh::writeElements( FILE* fp ) {
    for ( int i = 0; i < element_list_index; i++ ) {
        fwrite( element_list[ i ], sizeof( elements ), 1, fp );
    }
    return 1;
}

bool Mesh::writeMaps( FILE* fp ) {
    for ( int i = 0; i < map_list_index; i++ ) {
        Map& tmp_map = map_list[ i ];
        fwrite( map_list[ i ], sizeof( map ), 1, fp );
        fwrite( map_list[ i ]->map, sizeof( int ), map_list[ i ]->from->size * map_list[ i ]->dim, fp );
    }
    return 1;
}
bool Mesh::writeData( FILE* fp ) {
    for ( int i = 0; i < dat_list_index; i++ ) {
        fwrite( dat_list[ i ], sizeof( dat ), 1, fp );
        Data d = dat_list[ i ];
        fwrite( d->data, sizeof( double ), d->dim * d->set->size, fp );
    }
    double* pd = (double*)( dat_list[ 3 ]->data );
    return 1;
}

bool Mesh::readHeader( FILE* fp ) {
    fscanf( fp, "#emd %d %d %d %d %d %d\n", &element_list_size, &map_list_size, &dat_list_size,
            &element_list_index, &map_list_index, &dat_list_index );
    return 1;
}

bool Mesh::readElements( FILE* fp ) {
    for ( int i = 0; i < element_list_index; i++ ) {
        Elements tmp_element = (Elements)malloc( sizeof( elements ) );
        fread( tmp_element, sizeof( elements ), 1, fp );
        element_list[ i ] = tmp_element;
    }
    return 1;
}

bool Mesh::readMaps( FILE* fp ) {
    for ( int i = 0; i < map_list_index; i++ ) {
        // fwrite(map_list[i], sizeof(map), 1, fp);
        Map tmp_map = (Map)malloc( sizeof( map ) );
        fread( tmp_map, sizeof( map ), 1, fp );
        tmp_map->map = (int*)malloc( sizeof( int ) * tmp_map->from->size * tmp_map->dim );
        fread( tmp_map->map, sizeof( int ), tmp_map->from->size * tmp_map->dim, fp );
        map_list[ i ] = tmp_map;
    }
    return 1;
}
bool Mesh::readData( FILE* fp ) {
    for ( int i = 0; i < dat_list_index; i++ ) {
        Data tmp_dat = (Data)malloc( sizeof( dat ) );
        fread( tmp_dat, sizeof( dat ), 1, fp );
        tmp_dat->data = (char*)malloc( sizeof( double ) * tmp_dat->dim * tmp_dat->set->size );
        fread( tmp_dat->data, sizeof( double ), tmp_dat->dim * tmp_dat->set->size, fp );
        dat_list[ i ] = tmp_dat;
    }
    double* pd = (double*)( dat_list[ 3 ]->data );
    return 1;
}

#endif  // CodeExecutable
