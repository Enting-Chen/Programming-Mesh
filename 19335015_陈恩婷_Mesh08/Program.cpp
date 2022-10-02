#include "Program.h"

#include "Mesh.h"
#define _USE_MATH_DEFINES

#include <iostream>
using namespace std;

Program::Program( Mesh& s1 ) : s( s1 ) {
    // s = s1;
    this->dt = 0.05;
    this->h = 0.1;
    this->U = 1;
    this->D = 0.05;
}

Program::Program( Mesh& s1, double dt, double h, double U, double D ) : s( s1 ) {
    // s = s1;
    this->dt = dt;
    this->h = h;
    this->U = U;
    this->D = D;
}
Program::~Program() {
}
int Program::init() {  //需要修改
    int n = s.element_list[ 0 ]->size;
    double* dp = (double*)malloc( sizeof( double ) * ( n ) );
    double* dp3 = (double*)s.dat_list[ 3 ]->data;
    for ( int i = 0; i < n; i++ ) {
        dp[ i ] = 0.5 * sin( 2 * 3.1415926 * h * i );
    }
    s.makeData( s.element_list[ 0 ], 1, "double", (char*)dp, "data2_on_nodes" );
    return 0;
}
int Program::average( int d, int i, int j ) {
    return s.dat_list[ d ]->data[ i ] + s.dat_list[ d ]->data[ j ];
}

double Program::advection_diffusion( double* f ) {  //需要修改
    double fx;
    fx = *f - U * dt * ( *( f + 1 ) - *( f - 1 ) ) / ( 2 * h ) + D * dt * ( *( f + 1 ) + *( f - 1 ) - 2 * ( *f ) ) / ( h * h );
    // cout << fx << " ";
    return fx;
}
int Program::loop( double ( Program::*fx )( double* a ), int e, int d ) {
    int n = s.element_list[ e ]->size;
    double* f1 = (double*)s.dat_list[ d ]->data;
    double* pp = (double*)malloc( sizeof( double ) * n );
    memset( pp, 0, sizeof( double ) * n );
    for ( int i = 0; i < n; i++ ) {
        pp[ i ] = ( (double*)( s.dat_list[ d ]->data ) )[ i ];
    }
    for ( int i = 1; i < n - 1; i++ ) {
        f1[ i ] = advection_diffusion( pp + i );
    }
    f1[ n - 1 ] = pp[ n - 1 ] - U * dt * ( pp[ 1 ] - pp[ n - 2 ] ) / ( 2 * h ) + D * dt * ( pp[ 1 ] - 2 * pp[ n - 1 ] + pp[ n - 2 ] ) / ( h * h );
    f1[ 0 ] = f1[ n - 1 ];
    return 0;
}
int Program::program( int nstep, int e, int d ) {  //需要修改
    double tm = 0;
    int n = s.element_list[ e ]->size;
    for ( int j = 0; j < nstep; j++, tm += dt ) {
        loop( &Program::advection_diffusion, e, d );
    }
    return 0;
}