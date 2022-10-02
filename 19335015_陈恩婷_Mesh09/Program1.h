#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
//#include <vector>

class Mesh;

class Program1
{
	double gam, gm1, cfl, eps, mach, alpha, qinf[4];  //7 constants
	Mesh & s;  
	//4 elements: nodes,edges,bedges, cells
	//5 maps: cell2node,edge2node,edge2cell,bedge2node, bedge2cell
	//6 dat: bound(bedge,int), x(nodes), q(4 on cells), qold(4 on cells), adt(cell), res(4 on cell)

public:


	void initflow();

//Kernel functions
	void save(const double* q, double* qold);
	void area(const double* x1, const double* x2, const double* x3,const double* x4, const double* q, double* adt);
	void flux(const double* x1, const double* x2, const double* q1,
		const double* q2, const double* adt1, const double* adt2, double* res1, double* res2);
	void bcond(const double* x1, const double* x2, const double* q1,
		const double* adt1, double* res1, const int* bound);// Apply boundary conditions
	void update(const double* qold, double* q, double* res,	const double* adt, double* rms);

//Program
	int run(int step);

public:
	Program1(Mesh &ms);
	~Program1();

};

