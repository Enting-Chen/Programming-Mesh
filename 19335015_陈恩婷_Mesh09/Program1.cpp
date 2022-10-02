#include "Mesh.h"
#include "Program1.h"
#include <iostream>
using namespace std;

Program1::Program1(Mesh& ms) :s(ms) {
}
Program1::~Program1() {

}

//kernels
void Program1::initflow() {
	printf("initialising flow field \n");

	gam = 1.4f;
	gm1 = gam - 1.0f;
	cfl = 0.9f;
	eps = 0.05f;

	double mach = 0.4f;
	double alpha = 3.0f * atan(1.0f) / 45.0f;
	double p = 1.0f;
	double r = 1.0f;
	double u = sqrt(gam * p / r) * mach;
	double e = p / (r * gm1) + 0.5f * u * u;

	qinf[0] = r;
	qinf[1] = r * u;
	qinf[2] = 0.0f;
	qinf[3] = r * e;

	FILE* fp;
	int nnode,  ncell,nedge,  nbedge;

	if ((fp = fopen("./new_grid.dat", "r")) == NULL) {
		printf("can't open file new_grid.dat\n");
		exit(-1);
	}

	s.readrawHeader(fp, &nnode, &ncell, &nedge, &nbedge);
	
	s.makeElements(nnode, "nodes");   //0
	s.makeElements(nedge, "edges");   //1
	s.makeElements(nbedge, "bedges"); //2
	s.makeElements(ncell, "cells");   //3

	int* cell = (int *)malloc(4 * ncell * sizeof(int));
	int* edge = (int *)malloc(2 * nedge * sizeof(int));
	int* ecell = (int *)malloc(2 * nedge * sizeof(int));
	int* bedge = (int *)malloc(2 * nbedge * sizeof(int));
	int* becell = (int *)malloc(nbedge * sizeof(int));

	double* x = (double *)malloc(3 * nnode * sizeof(double));
	double* q = (double *)malloc(4 * ncell * sizeof(double));
	double* qold = (double *)malloc(4 * ncell * sizeof(double));
	double* res = (double *)malloc(4 * ncell * sizeof(double));
	double* adt = (double *)malloc(ncell * sizeof(double));
	int* bound = (int *)malloc(nbedge * sizeof(int));

	s.readrawNode(fp,  nnode, x);
	s.readrawCell(fp,  ncell, cell);
	s.readrawEdge(fp, nedge, edge, ecell);
	s.readrawBEdge(fp, nbedge, bedge, becell, bound);


	for (int n = 0; n < ncell; n++) { //ncell
		for (int m = 0; m < 4; m++) {
			 q[4 * n + m] = qinf[m];
			 res[4 * n + m] = 0.0f;
		}
	}

	s.makeMap(s.element_list[1], s.element_list[0], 2, edge , "pedge");
	s.makeMap(s.element_list[1], s.element_list[3], 2, ecell, "pecell");
	s.makeMap(s.element_list[2], s.element_list[0], 2, bedge, "pbedge");
	s.makeMap(s.element_list[2], s.element_list[3], 1, becell, "pbecell");
	s.makeMap(s.element_list[3], s.element_list[0], 4, cell, "pcell");

	s.makeData(s.element_list[2], 1, "int", (char*)bound, "p_bound");
	s.makeData(s.element_list[0], 3, "double", (char*)x, "p_x");
	s.makeData(s.element_list[3], 4, "double", (char*)q, "p_q");
	s.makeData(s.element_list[3], 4, "double", (char*)qold, "p_qold");
	s.makeData(s.element_list[3], 1, "double", (char*)adt, "p_adt");
	s.makeData(s.element_list[3], 4, "double", (char*)res, "p_res");

	return;
}

void Program1::save(const double* q, double* qold) {
	for (int n = 0; n < 4; n++) qold[n] = q[n];
}

void Program1::area(const double* x1, const double* x2, const double* x3,
	const double* x4, const double* q, double* adt) {  //loop on cells
	double dx, dy, ri, u, v, c;

	ri = 1.0f / q[0];
	u = ri * q[1];
	v = ri * q[2];
	c = sqrt(gam * gm1 * (ri * q[3] - 0.5f * (u * u + v * v)));

	dx = x2[0] - x1[0];
	dy = x2[1] - x1[1];
	*adt = fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);

	dx = x3[0] - x2[0];
	dy = x3[1] - x2[1];
	*adt += fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);

	dx = x4[0] - x3[0];
	dy = x4[1] - x3[1];
	*adt += fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);

	dx = x1[0] - x4[0];
	dy = x1[1] - x4[1];
	*adt += fabs(u * dy - v * dx) + c * sqrt(dx * dx + dy * dy);

	*adt = (*adt) / cfl;	
}

void Program1::flux(const double* x1, const double* x2, const double* q1,
	const double* q2, const double* adt1, const double* adt2,
	double* res1, double* res2) {  //loop on edges
	double dx, dy, mu, ri, p1, vol1, p2, vol2, f;

	dx = x1[0] - x2[0];
	dy = x1[1] - x2[1];

	ri = 1.0f / q1[0];
	p1 = gm1 * (q1[3] - 0.5f * ri * (q1[1] * q1[1] + q1[2] * q1[2]));
	vol1 = ri * (q1[1] * dy - q1[2] * dx);

	ri = 1.0f / q2[0];
	p2 = gm1 * (q2[3] - 0.5f * ri * (q2[1] * q2[1] + q2[2] * q2[2]));
	vol2 = ri * (q2[1] * dy - q2[2] * dx);

	mu = 0.5f * ((*adt1) + (*adt2)) * eps;

	f = 0.5f * (vol1 * q1[0] + vol2 * q2[0]) + mu * (q1[0] - q2[0]);
	res1[0] += f;
	res2[0] -= f;
	f = 0.5f * (vol1 * q1[1] + p1 * dy + vol2 * q2[1] + p2 * dy) +
		mu * (q1[1] - q2[1]);
	res1[1] += f;
	res2[1] -= f;
	f = 0.5f * (vol1 * q1[2] - p1 * dx + vol2 * q2[2] - p2 * dx) +
		mu * (q1[2] - q2[2]);
	res1[2] += f;
	res2[2] -= f;
	f = 0.5f * (vol1 * (q1[3] + p1) + vol2 * (q2[3] + p2)) + mu * (q1[3] - q2[3]);
	res1[3] += f;
	res2[3] -= f;
}

void Program1::bcond(const double* x1, const double* x2, const double* q1,
	const double* adt1, double* res1, const int* bound) {  //loop on bedges
	double dx, dy, mu, ri, p1, vol1, p2, vol2, f;

	dx = x1[0] - x2[0];
	dy = x1[1] - x2[1];

	ri = 1.0f / q1[0];
	p1 = gm1 * (q1[3] - 0.5f * ri * (q1[1] * q1[1] + q1[2] * q1[2]));

	if (*bound == 1) {
		res1[1] += +p1 * dy;
		res1[2] += -p1 * dx;
	}
	else {
		vol1 = ri * (q1[1] * dy - q1[2] * dx);

		ri = 1.0f / qinf[0];
		p2 = gm1 * (qinf[3] - 0.5f * ri * (qinf[1] * qinf[1] + qinf[2] * qinf[2]));
		vol2 = ri * (qinf[1] * dy - qinf[2] * dx);

		mu = (*adt1) * eps;

		f = 0.5f * (vol1 * q1[0] + vol2 * qinf[0]) + mu * (q1[0] - qinf[0]);
		res1[0] += f;
		f = 0.5f * (vol1 * q1[1] + p1 * dy + vol2 * qinf[1] + p2 * dy) +
			mu * (q1[1] - qinf[1]);
		res1[1] += f;
		f = 0.5f * (vol1 * q1[2] - p1 * dx + vol2 * qinf[2] - p2 * dx) +
			mu * (q1[2] - qinf[2]);
		res1[2] += f;
		f = 0.5f * (vol1 * (q1[3] + p1) + vol2 * (qinf[3] + p2)) +
			mu * (q1[3] - qinf[3]);
		res1[3] += f;
	}
}

void Program1::update(const double* qold, double* q, double* res,
	const double* adt, double* rms) {  //loop on cells
	double del, adti;

	adti = 1.0f / (*adt);
	for (int n = 0; n < 4; n++) {
		del = adti * res[n];
		q[n] = qold[n] - del;
		res[n] = 0.0f;
		*rms += del * del;
	}
}
int Program1::run(int step)
{
	initflow();

	int niter = 100;
	int n = s.element_list[3]->size;  //ncells
	int m = s.element_list[1]->size;  //nedges
	int bm = s.element_list[2]->size;  //nbedges
	double rms=0.0;
	for (int iter = 1; iter <= niter; iter++) {
		for (int i = 0; i <n ; i++) {
			save((double*)s.dat_list[2]->data + 4*i, (double*)s.dat_list[3]->data + 4*i );
		}
		for (int k = 0; k < 2; k++) {
			for (int i = 0; i < n; i++) { //////////////
				area((double*)s.dat_list[1]->data+3*(s.map_list[4]->map[4*i]), (double*)s.dat_list[1]->data + 3*(s.map_list[4]->map[4*i + 1]), (double*)s.dat_list[1]->data+3*(s.map_list[4]->map[4 * i + 2]),
					(double*)s.dat_list[1]->data+3*(s.map_list[4]->map[4*i+3]), (double*)s.dat_list[2]->data + 4*i, (double*)s.dat_list[4]->data + i);
			}
			for (int i = 0; i < m; i++) { //////////////
				flux((double*)s.dat_list[1]->data+3*(s.map_list[0]->map[2*i]), (double*)s.dat_list[1]->data +3*(s.map_list[0]->map[2*i+1]), (double*)s.dat_list[2]->data+4*(s.map_list[1]->map[2*i]),
					(double*)s.dat_list[2]->data+4*(s.map_list[1]->map[2*i+1]), (double*)s.dat_list[4]->data+s.map_list[1]->map[2*i], (double*)s.dat_list[4]->data + s.map_list[1]->map[2 * i + 1],
					(double*)s.dat_list[5]->data + 4*(s.map_list[1]->map[2 * i]), (double*)s.dat_list[5]->data+4*(s.map_list[1]->map[2 * i + 1]));
			}
			for (int i = 0; i < bm; i++) {  //////////////////
				bcond((double*)s.dat_list[1]->data + 3 * (s.map_list[2]->map[2 * i]), (double*)s.dat_list[1]->data + 3 * (s.map_list[2]->map[2 * i + 1]), (double*)s.dat_list[2]->data + 4*(s.map_list[3]->map[i]),
					(double*)s.dat_list[4]->data + s.map_list[3]->map[i], (double*)s.dat_list[5]->data + 4 * (s.map_list[3]->map[i]), (int*)s.dat_list[0]->data + i);
			}
			rms = 0.0;

			for (int i = 0; i < n; i++) {
				update((double*)s.dat_list[3]->data + 4*i, (double*)s.dat_list[2]->data + 4*i, (double*)s.dat_list[5]->data + 4*i,
					(double*)s.dat_list[4]->data + i, &rms);
			}
			double* p9 = (double*)s.dat_list[2]->data;
		}

		rms = sqrt(rms / (double)n);
		if (iter % 10 == 0) {
			printf(" %d  %10.5e \n", iter, rms);
			//printf("\tsave: %f\n", save);
			//printf("\tarea: %f\n", area);
			//printf("\tflux_res: %f\n", flux_res);
			//printf("\tperem: %f\n", perem);
			//printf("\tupdate: %f\n", update);
			char buf[50];
			sprintf(buf, "out%d.vtk", iter);
			s.savevtk(buf);
		}
	
	}
	return 0;
}