#include "MeshMemory.h"
#ifdef CodeExecutable
    #include <cstring>
    #include <iostream>
using namespace std;

MeshMemory::MeshMemory() {
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

MeshMemory::MeshMemory( char* fname ) {
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

MeshMemory::~MeshMemory() {
    free(element_list);
    free(map_list);
    free(dat_list);
}

////////////////////////////////////////////////
Elements MeshMemory::makeElements( int ele_size, char const* ele_name ) {
    Elements ele = (Elements)malloc( sizeof( elements ) );
    ele->name = ele_name;
    ele->size = ele_size;
    ele->index = element_list_index;
    element_list[ element_list_index++ ] = ele;
    return ele;
}
Map MeshMemory::makeMap( Elements map_from, Elements map_to, int map_dim, int* map_map, char const* map_name ) {
    printf( "hello\n" );
    // cout << "hello" << endl;
    // cout << map_dim << endl;
    // cout << arr_cnt << endl;

    int arr_cnt = map_from->size;
    Map mapi = (Map)malloc( sizeof( map ) );
    mapi->index = map_list_index;
    mapi->from = map_from;
    mapi->to = map_to;
    mapi->dim = map_dim;
    mapi->name = map_name;
    mapi->map = map_map;
    map_list[ map_list_index++ ] = (Map)mapi;
    return (Map)mapi;
}
Data MeshMemory::makeData( Elements data_set, int data_dim, char const* data_type, char* data_data, char const* data_name ) {
    Data data = (Data)malloc( sizeof( dat ) );
    data->index = dat_list_index;
    data->set = data_set;
    data->dim = data_dim;
    data->size = 8;
    data->data = data_data;
    data->name = data_name;
    dat_list[ dat_list_index++ ] = data;
    return data;
}

/////////////////////////////////////////////////

bool MeshMemory::readfromfile( const char* fileName ) {
    FILE* fp;
    if ( ( fp = fopen( fileName, "rb" ) ) == NULL ) {
        printf( "can't open file\n" );
        return 0;
    }

    readHeader( fp );
    readElements( fp );
    readMaps( fp );
    readData( fp );

    fclose( fp );
    return true;
}

bool MeshMemory::savetofile( const char* fileName ) {
    printf( "writing in grid \n" );
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

bool MeshMemory::writeHeader( FILE* fp ) {
    fprintf( fp, "#emd %d %d %d %d %d %d\n", element_list_size, map_list_size, dat_list_size,
             element_list_index, map_list_index, dat_list_index );
    return 1;
}

bool MeshMemory::writeElements( FILE* fp ) {
    fwrite( element_list, sizeof( elements ), element_list_index, fp );
    return 1;
}

bool MeshMemory::writeMaps( FILE* fp ) {
    fwrite( map_list, sizeof( map ), map_list_index, fp );
    for ( int i = 0; i < map_list_index; i++ ) {
        Map map = map_list[ i ];
        // fwrite(map_list[i], sizeof(map), 1, fp);
        fwrite( map->map, sizeof( int ), map->from->size * map->dim, fp );
    }
    return 1;
}
bool MeshMemory::writeData( FILE* fp ) {
    fwrite( dat_list, sizeof( dat ), dat_list_index, fp );
    for ( int i = 0; i < dat_list_index; i++ ) {
        // fwrite(dat_list[i], sizeof(dat), 1, fp);
        Data d = dat_list[ i ];
        fwrite( d->data, sizeof( double ), d->dim * d->set->size, fp );
    }
    double* pd = (double*)( dat_list[ 3 ]->data );
    for ( int i = 0; i < 16; i++ ) {
        cout << pd[ i ] << endl;
    }
    return 1;
}

bool MeshMemory::readHeader( FILE* fp ) {
    fscanf( fp, "#emd %d %d %d %d %d %d\n", &element_list_size, &map_list_size, &dat_list_size,
            &element_list_index, &map_list_index, &dat_list_index );
    return 1;
}

bool MeshMemory::readElements( FILE* fp ) {
    fread( element_list, sizeof( elements ), element_list_index, fp );
    //for ( int i = 0; i < element_list_index; i++ ) {
        //cout << element_list[ i ]->name << " " << element_list[ i ]->size << endl;
    //}
    return 1;
}

bool MeshMemory::readMaps( FILE* fp ) {
    fread( map_list, sizeof( map ), map_list_index, fp );
    for ( int i = 0; i < map_list_index; i++ ) {
        // fwrite(map_list[i], sizeof(map), 1, fp);
        Map map = map_list[ i ];
        map->map = (int*)malloc( sizeof( int ) * map->from->size * map->dim );
        fread( map->map, sizeof( int ), map->from->size * map->dim, fp );
    }
    return 1;
}
bool MeshMemory::readData( FILE* fp ) {
    fread( dat_list, sizeof( dat ), dat_list_index, fp );
    for ( int i = 0; i < dat_list_index; i++ ) {
        // fwrite(dat_list[i], sizeof(dat), 1, fp);
        Data d = dat_list[ i ];
        d->data = (char*)malloc( sizeof( double ) * d->dim * d->set->size );
        fread( d->data, sizeof( double ), d->dim * d->set->size, fp );
        cout << d->name << d->dim << " " << d->set->size << endl;
    }
    double* pd = (double*)( dat_list[ 3 ]->data );
    for ( int i = 0; i < 16; i++ ) {
        cout << pd[ i ] << endl;
    }
    return 1;
}
#endif  // MyCodeExe
