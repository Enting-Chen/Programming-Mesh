#include "MeshVTK3Alg.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>
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
    // put your code here
    readSectionLines( fp );  // ��ȡͷ
    char str[ 100 ];
    while ( fscanf( fp, "%s", str ) != EOF ) {
        if ( strcmp( str, "POINTS" ) == 0 ) {
            readPoints( fp );
        }
        else if ( strcmp( str, "CELLS" ) == 0 ) {
            readCells( fp );
        }
        else if ( strcmp( str, "CELL_TYPES" ) == 0 ) {
            readCellType( fp );
        }
        else if ( strcmp( str, "CELL_DATA" ) == 0 ) {
            readCellData( fp );
        }
    }
    init();
    //cout << nedge << " " << nbedge << endl;
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

    // put your code here
    writeSectionLines( fp );
    writePoints( fp );
    writeCells( fp );
    writeCellType( fp );
    writeCellData( fp );

    for (int n = 0; n < nbedge; n++)
    {
        fprintf(fp, "%d %d %d %d \n", bedge[2 * n], bedge[2 * n + 1],
            becell[n], bound[n]);
    }
    fclose( fp );
    return true;
}

void MeshVTK::init() {
    //cout << ncell << endl;
    map<pair<int, int>, int> bedge_map;
    map<pair<int, int>, pair<int, int>> edge_map;
    pair<int, int> p, p1, p2;
    for ( int i = 0; i < ncell; i++ ) {
        for ( int j = 0; j < 4; j++ ) {
            p2.second = p.first = cells[ 4 * i + j % 4 ];
            p2.first = p.second = cells[ 4 * i + ( j + 1 ) % 4 ];
            if ( bedge_map.find( p ) != bedge_map.end() ) {
                p1.first = bedge_map[ p ];
                bedge_map.erase( p );
                p1.second = i;
                edge_map.insert( make_pair( p, p1 ) );
            }
            else if ( bedge_map.find( p2 ) != bedge_map.end() ) {
                p1.first = bedge_map[ p2 ];
                bedge_map.erase( p2 );
                p1.second = i;
                edge_map.insert( make_pair( p, p1 ) );
            }
            else {
                bool flag = false;
                for (auto& e : bedge_map) {
                    double a = xyz[3 * e.first.first];
                    double b = xyz[3 * e.first.first + 1];
                    double c = xyz[3 * p.first];
                    double d = xyz[3 * p.first + 1];
                    double e1 = xyz[3 * e.first.second ];
                    double f = xyz[3 * e.first.second + 1];
                    double g = xyz[3 * p.second];
                    double h = xyz[3 * p.second + 1];
                    if ( (make_pair(a, b) == make_pair(c, d) && make_pair(e1, f) == make_pair(g, h) ) 
                        || (make_pair(a, b) == make_pair(g, h) && make_pair(e1, f) == make_pair(c, d)) ){
                            p1.first = bedge_map[e.first];
                            bedge_map.erase(e.first);
                            p1.second = i;
                            edge_map.insert(make_pair(p, p1));
                            flag = true;
                            break;
                    }
                }
                if (!flag) {
                    bedge_map.insert( make_pair( p2, i ) );
                }     
            }
        }
    }

    cout << edge_map.size() << " " << bedge_map.size() << endl;

    nedge = edge_map.size();
    nbedge = bedge_map.size();

    edge = (int*)malloc( 2 * nedge * sizeof( int ) );
    ecell = (int*)malloc( 2 * nedge * sizeof( int ) );
    bedge = (int*)malloc( 2 * nbedge * sizeof( int ) );
    becell = (int*)malloc( nbedge * sizeof( int ) );
    bound = (int*)malloc( nbedge * sizeof( int ) );

    q = (real*)malloc( 4 * ncell * sizeof( real ) );
    qold = (real*)malloc( 4 * ncell * sizeof( real ) );
    res = (real*)malloc( 4 * ncell * sizeof( real ) );
    adt = (real*)malloc( ncell * sizeof( real ) );

    int i = 0;
    for ( auto& e : edge_map ) {
        edge[ i ] = e.first.first;
        edge[ i + 1 ] = e.first.second;
        ecell[ i ] = e.second.first;
        ecell[ i + 1 ] = e.second.second;
        i += 2;
    }

    i = 0;
    //int j = 1;
    set<int> bedge_set;
    for ( auto& e : bedge_map ) {
        bedge[ 2 * i ] = e.first.first;
        bedge[ 2 * i + 1 ] = e.first.second;
        becell[ i ] = e.second;
        if ( bedge[2*i] <= 400 && bedge[2*i] >= 200) {
            bound[ i ] = 1;
        }
        else{
            bound[ i ] = 2;
        }
        bedge_set.insert(e.first.first);
        bedge_set.insert(e.first.second);
        i++;
    }

    // set variables for graph coloring
    cell2edge = (int*)malloc( 4 * ncell * sizeof( int ) );
    for ( int i = 0; i < 4 * ncell; ++i ) {
        cell2edge[ i ] = -1;
    }
    edge2color = (int*)malloc( nedge * sizeof( int ) );
    for ( int i = 0; i < nedge; ++i ) {
        edge2color[ i ] = -1;
    }
    rms = 0.0;
}

bool MeshVTK::readSectionLines( FILE* fp ) {
    char str[ 100 ];
    fscanf( fp, "%[^\n]%*c", str );
    int len = strlen( str );
    vtkVer = str[ len - 3 ] - 48;               // ���汾��
    vtkSubVer = str[ len - 1 ] - 48;            // ���汾��
    fscanf( fp, "%[^\n]%*c", vtkDescription );  // vtk��Ϣ
    fscanf( fp, "%[^\n]%*c", str );             // ASCII
    if ( strcmp( str, "ASCII" ) == 0 ) {
        ascii = 1;
    }
    fscanf( fp, "%[^\n]%*c", dataset );  //������Ϣ
    return 1;
}
bool MeshVTK::readPoints( FILE* fp ) {
    char str[ 50 ];
    fscanf( fp, "%d %s\n", &npoint, str );
    if ( strcmp( str, "double" ) == 0 ) {
        xyz = (double*)malloc( 3 * npoint * sizeof( double ) );
    }
    for ( int n = 0; n < npoint; n++ ) {
        fscanf( fp, "%lf %lf %lf\n", &xyz[ 3 * n ], &xyz[ 3 * n + 1 ], &xyz[ 3 * n + 2 ] );
    }
    return 1;
}
bool MeshVTK::readCells( FILE* fp ) {
    fscanf( fp, "%d %d\n", &ncell, &cellsize );
    cells = (int*)malloc( ncell * 4 * sizeof( int ) );
    csize = ncell;
    int count = 0;
    while ( count < ncell * 4 ) {
        fscanf( fp, "%d\n", &cells[ count ] );
        int vex = 4;
        // count++;
        while ( vex ) {
            vex--;
            fscanf( fp, "%d\n", &cells[ count++ ] );
        }
    }
    return 1;
}
bool MeshVTK::readCellType( FILE* fp ) {
    fscanf( fp, "%d\n", &ncell );
    celltypes = (int*)malloc( ncell * sizeof( int ) );
    for ( int n = 0; n < ncell; n++ ) {
        fscanf( fp, "%d \n", &celltypes[ n ] );
    }
    return 1;
}

bool MeshVTK::readCellData( FILE* fp ) {
    char str[ 100 ];
    fscanf( fp, "%d\n", &ncell );
    csize = ncell;
    celldata = (int*)malloc( ncell * sizeof( int ) );
    fscanf( fp, "%[^\n]%*c", str );
    ncComp = str[ strlen( str ) - 1 ] - 48;
    fscanf( fp, "%[^\n]%*c", str );
    for ( int n = 0; n < ncell; n++ ) {
        fscanf( fp, "%d \n", &celldata[ n ] );
    }
    return 1;
}

bool MeshVTK::readPointData( FILE* fp ) {
    char str[ 100 ];
    fscanf( fp, "%d\n", &npoint );
    psize = npoint;
    pointdata = (int*)malloc( npoint * sizeof( int ) );
    fscanf( fp, "%[^\n]%*c", str );
    npComp = str[ strlen( str ) - 1 ] - 48;
    fscanf( fp, "%[^\n]%*c", str );
    for ( int n = 0; n < npoint; n++ ) {
        fscanf( fp, "%d \n", &pointdata[ n ] );
    }
    return 1;
}

bool MeshVTK::writeSectionLines( FILE* fp ) {
    fprintf( fp, "# vtk DataFile Version %d.%d\n", vtkVer, vtkSubVer );
    fprintf( fp, "%s\n", vtkDescription );
    fprintf( fp, "ASCII\n" );
    fprintf( fp, "%s\n", dataset );
    return 1;
}

bool MeshVTK::writePoints( FILE* fp ) {
    fprintf( fp, "POINTS %d double\n", npoint );
    for ( int n = 0; n < npoint; n++ ) {
        fprintf( fp, "%lf %lf %lf\n", xyz[ 3 * n ], xyz[ 3 * n + 1 ], xyz[ 3 * n + 2 ] );
    }
    return 1;
}

bool MeshVTK::writeCells( FILE* fp ) {
    fprintf( fp, "\nCELLS %d %d\n", ncell, cellsize );
    for ( int n = 0; n < 4; n++ ) {
        fprintf( fp, "%d %d \n", cells[ 2 * n ], cells[ 2 * n + 1 ] );
    }
    for ( int n = 4; n < 12; n++ ) {
        fprintf( fp, "%d %d %d \n", cells[ 3 * n - 4 ], cells[ 3 * n - 3 ], cells[ 3 * n - 2 ] );
    }
    for ( int n = 12; n < ncell; n++ ) {
        fprintf( fp, "%d %d %d %d \n", cells[ 4 * n - 16 ], cells[ 4 * n - 15 ], cells[ 4 * n - 14 ], cells[ 4 * n - 13 ] );
    }
    return 1;
}
bool MeshVTK::writeCellType( FILE* fp ) {
    fprintf( fp, "\nCELL_TYPES %d \n", ncell );
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d \n", celltypes[ n ] );
    }
    return 1;
}
bool MeshVTK::writeCellData( FILE* fp ) {
    fprintf( fp, "\nCELL_DATA %d \n", ncell );
    fprintf( fp, "SCALARS CellEntityIds int %d\n", ncComp );
    fprintf( fp, "LOOKUP_TABLE default\n" );
    for ( int n = 0; n < ncell; n++ ) {
        fprintf( fp, "%d \n", celldata[ n ] );
    }
    return 1;
}

bool MeshVTK::writePointData( FILE* fp ) {
    return 1;
}

// kernels
void MeshVTK::initflow() {
    printf( "initialising flow field \n" );

    gam = 1.4f;
    gm1 = gam - 1.0f;
    cfl = 0.9f;
    eps = 0.05f;

    real mach = 0.4f;
    real alpha = 3.0f * atan( 1.0f ) / 45.0f;
    real p = 1.0f;
    real r = 1.0f;
    real u = sqrt( gam * p / r ) * mach;
    real e = p / ( r * gm1 ) + 0.5f * u * u;

    qinf[ 0 ] = r;
    qinf[ 1 ] = r * u;
    qinf[ 2 ] = 0.0f;
    qinf[ 3 ] = r * e;

    // std::cout << qinf[ 0 ] << " " << qinf[ 1 ] << " " << qinf[ 2 ] << " " << qinf[ 3 ] << std::endl;
    for ( int n = 0; n < ncell; n++ ) {
        for ( int m = 0; m < 4; m++ ) {
            q[ 4 * n + m ] = qinf[ m ];
            res[ 4 * n + m ] = 0.0f;
        }
    }
    // std::cout << q[ 0 ] << " " << q[ 1 ] << " " << q[ 2 ] << " " << q[ 3 ] << std::endl;
    return;
}

int MeshVTK::sav( int i ) {
    for ( int n = 0; n < 4; n++ ) qold[ 4 * i + n ] = q[ 4 * i + n ];
    return 0;
}

int MeshVTK::area( int i ) {  // loop on cells
    double dx, dy, ri, u, v, c;

    ri = 1.0f / q[ 4 * i + 0 ];
    u = ri * q[ 4 * i + 1 ];
    v = ri * q[ 4 * i + 2 ];
    c = sqrt( gam * gm1 * ( ri * q[ 4 * i + 3 ] - 0.5f * ( u * u + v * v ) ) );

    dx = xyz[ cells[ 4 * i + 1 ] * 3 + 0 ] - xyz[ cells[ 4 * i + 0 ] * 3 + 0 ];
    dy = xyz[ cells[ 4 * i + 1 ] * 3 + 1 ] - xyz[ cells[ 4 * i + 0 ] * 3 + 1 ];
    adt[ i ] = fabs( u * dy - v * dx ) + c * sqrt( dx * dx + dy * dy );

    dx = xyz[ cells[ 4 * i + 2 ] * 3 + 0 ] - xyz[ cells[ 4 * i + 1 ] * 3 + 0 ];
    dy = xyz[ cells[ 4 * i + 2 ] * 3 + 1 ] - xyz[ cells[ 4 * i + 1 ] * 3 + 1 ];
    adt[ i ] += fabs( u * dy - v * dx ) + c * sqrt( dx * dx + dy * dy );

    dx = xyz[ cells[ 4 * i + 3 ] * 3 + 0 ] - xyz[ cells[ 4 * i + 2 ] * 3 + 0 ];
    dy = xyz[ cells[ 4 * i + 3 ] * 3 + 1 ] - xyz[ cells[ 4 * i + 2 ] * 3 + 1 ];
    adt[ i ] += fabs( u * dy - v * dx ) + c * sqrt( dx * dx + dy * dy );

    dx = xyz[ cells[ 4 * i + 0 ] * 3 + 0 ] - xyz[ cells[ 4 * i + 3 ] * 3 + 0 ];
    dy = xyz[ cells[ 4 * i + 0 ] * 3 + 1 ] - xyz[ cells[ 4 * i + 3 ] * 3 + 1 ];
    adt[ i ] += fabs( u * dy - v * dx ) + c * sqrt( dx * dx + dy * dy );

    adt[ i ] = adt[ i ] / cfl;

    return 0;
}

int MeshVTK::flux( int i ) {  // loop on edges
    double dx, dy, mu, ri, p1, vol1, p2, vol2, f;

    dx = xyz[ edge[ 2 * i + 0 ] * 3 + 0 ] - xyz[ edge[ 2 * i + 1 ] * 3 + 0 ];
    dy = xyz[ edge[ 2 * i + 0 ] * 3 + 1 ] - xyz[ edge[ 2 * i + 1 ] * 3 + 1 ];

    ri = 1.0f / q[ ecell[ 2 * i + 0 ] * 4 + 0 ];
    p1 = gm1 * ( q[ ecell[ 2 * i + 0 ] * 4 + 3 ] - 0.5f * ri * ( q[ ecell[ 2 * i + 0 ] * 4 + 1 ] * q[ ecell[ 2 * i + 0 ] * 4 + 1 ] + q[ ecell[ 2 * i + 0 ] * 4 + 2 ] * q[ ecell[ 2 * i + 0 ] * 4 + 2 ] ) );
    vol1 = ri * ( q[ ecell[ 2 * i + 0 ] * 4 + 1 ] * dy - q[ ecell[ 2 * i + 0 ] * 4 + 2 ] * dx );

    ri = 1.0f / q[ ecell[ 2 * i + 1 ] * 4 + 0 ];
    p2 = gm1 * ( q[ ecell[ 2 * i + 1 ] * 4 + 3 ] - 0.5f * ri * ( q[ ecell[ 2 * i + 1 ] * 4 + 1 ] * q[ ecell[ 2 * i + 1 ] * 4 + 1 ] + q[ ecell[ 2 * i + 1 ] * 4 + 2 ] * q[ ecell[ 2 * i + 1 ] * 4 + 2 ] ) );
    vol2 = ri * ( q[ ecell[ 2 * i + 1 ] * 4 + 1 ] * dy - q[ ecell[ 2 * i + 1 ] * 4 + 2 ] * dx );

    mu = 0.5f * ( adt[ ecell[ 2 * i + 0 ] ] + adt[ ecell[ 2 * i + 1 ] ] ) * eps;

    f = 0.5f * ( vol1 * q[ ecell[ 2 * i + 0 ] * 4 + 0 ] + vol2 * q[ ecell[ 2 * i + 1 ] * 4 + 0 ] ) + mu * ( q[ ecell[ 2 * i + 0 ] * 4 + 0 ] - q[ ecell[ 2 * i + 1 ] * 4 + 0 ] );
    res[ ecell[ 2 * i + 0 ] * 4 + 0 ] += f;
    res[ ecell[ 2 * i + 1 ] * 4 + 0 ] -= f;
    f = 0.5f * ( vol1 * q[ ecell[ 2 * i + 0 ] * 4 + 1 ] + p1 * dy + vol2 * q[ ecell[ 2 * i + 1 ] * 4 + 1 ] + p2 * dy ) + mu * ( q[ ecell[ 2 * i + 0 ] * 4 + 1 ] - q[ ecell[ 2 * i + 1 ] * 4 + 1 ] );
    res[ ecell[ 2 * i + 0 ] * 4 + 1 ] += f;
    res[ ecell[ 2 * i + 1 ] * 4 + 1 ] -= f;
    f = 0.5f * ( vol1 * q[ ecell[ 2 * i + 0 ] * 4 + 2 ] - p1 * dx + vol2 * q[ ecell[ 2 * i + 1 ] * 4 + 2 ] - p2 * dx ) + mu * ( q[ ecell[ 2 * i + 0 ] * 4 + 2 ] - q[ ecell[ 2 * i + 1 ] * 4 + 2 ] );
    res[ ecell[ 2 * i + 0 ] * 4 + 2 ] += f;
    res[ ecell[ 2 * i + 1 ] * 4 + 2 ] -= f;
    f = 0.5f * ( vol1 * ( q[ ecell[ 2 * i + 0 ] * 4 + 3 ] + p1 ) + vol2 * ( q[ ecell[ 2 * i + 1 ] * 4 + 3 ] + p2 ) ) + mu * ( q[ ecell[ 2 * i + 0 ] * 4 + 3 ] - q[ ecell[ 2 * i + 1 ] * 4 + 3 ] );
    res[ ecell[ 2 * i + 0 ] * 4 + 3 ] += f;
    res[ ecell[ 2 * i + 1 ] * 4 + 3 ] -= f;
    return 0;
}

int MeshVTK::bcond( int i ) {  // loop on bedges
    double dx, dy, mu, ri, p1, vol1, p2, vol2, f;

    dx = xyz[ bedge[ 2 * i + 0 ] * 3 + 0 ] - xyz[ bedge[ 2 * i + 1 ] * 3 + 0 ];
    dy = xyz[ bedge[ 2 * i + 0 ] * 3 + 1 ] - xyz[ bedge[ 2 * i + 1 ] * 3 + 1 ];

    ri = 1.0f / q[ becell[ i ] * 4 + 0 ];
    p1 = gm1 * ( q[ becell[ i ] * 4 + 3 ] - 0.5f * ri * ( q[ becell[ i ] * 4 + 1 ] * q[ becell[ i ] * 4 + 1 ] + q[ becell[ i ] * 4 + 2 ] * q[ becell[ i ] * 4 + 2 ] ) );

    if ( bound[ i ] == 1 ) {  // Far-field
        res[ becell[ i ] * 4 + 1 ] += +p1 * dy;
        res[ becell[ i ] * 4 + 2 ] += -p1 * dx;
    }
    else {
        vol1 = ri * ( q[ becell[ i ] * 4 + 1 ] * dy - q[ becell[ i ] * 4 + 2 ] * dx );

        ri = 1.0f / qinf[ 0 ];
        p2 = gm1 * ( qinf[ 3 ] - 0.5f * ri * ( qinf[ 1 ] * qinf[ 1 ] + qinf[ 2 ] * qinf[ 2 ] ) );
        vol2 = ri * ( qinf[ 1 ] * dy - qinf[ 2 ] * dx );

        mu = adt[ becell[ i ] ] * eps;

        f = 0.5f * ( vol1 * q[ becell[ i ] * 4 + 0 ] + vol2 * qinf[ 0 ] ) + mu * ( q[ becell[ i ] * 4 + 0 ] - qinf[ 0 ] );
        res[ becell[ i ] * 4 + 0 ] += f;
        f = 0.5f * ( vol1 * q[ becell[ i ] * 4 + 1 ] + p1 * dy + vol2 * qinf[ 1 ] + p2 * dy ) + mu * ( q[ becell[ i ] * 4 + 1 ] - qinf[ 1 ] );
        res[ becell[ i ] * 4 + 1 ] += f;
        f = 0.5f * ( vol1 * q[ becell[ i ] * 4 + 2 ] - p1 * dx + vol2 * qinf[ 2 ] - p2 * dx ) + mu * ( q[ becell[ i ] * 4 + 2 ] - qinf[ 2 ] );
        res[ becell[ i ] * 4 + 2 ] += f;
        f = 0.5f * ( vol1 * ( q[ becell[ i ] * 4 + 3 ] + p1 ) + vol2 * ( qinf[ 3 ] + p2 ) ) + mu * ( q[ becell[ i ] * 4 + 3 ] - qinf[ 3 ] );
        res[ becell[ i ] * 4 + 3 ] += f;
    }
    return 0;
}

int MeshVTK::update( int i ) {  // loop on cells
    double del, adti;

    adti = 1.0f / adt[ i ];

    for ( int n = 0; n < 4; n++ ) {
        del = adti * res[ 4 * i + n ];
        q[ 4 * i + n ] = qold[ 4 * i + n ] - del;
        res[ 4 * i + n ] = 0.0f;
        rms += del * del;
    }
    // std::cout << "rms" << rms << std::endl;
    return 0;
}
