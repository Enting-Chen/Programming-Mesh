#include "Mesh.h"
#include "Program.h"

Program::Program(Mesh & s1)
{
	s = s1;
	this->dt = 0.05;
	this->h = 0.1;
	this->U = 1;
	this->D = 0.05;
}

Program::Program(Mesh& s1, double dt, double h, double U, double D) {
	s = s1;
	this->dt = dt;
	this->h = h;
	this->U = U;
	this->D = D;
}
Program::~Program()
{
}
int Program::init() {  //需要修改
	int n = s.element_list[0]->size;
	double * dp = (double*)malloc(sizeof(double) * (n));
	double* dp3 = (double*)s.dat_list[3]->data;
	double tm = 0.0;
	for (int i = 0; i < n; i++) {
		dp[i] = exp(-4*3.1415926 * 3.1415926*D*tm)*0.5*sin(2*3.1415926*(h*(i)-U*tm));
		dp3[i] = dp[i];
		//tm = tm + dt;
	}

	s.makeData(s.element_list[0], 1, "double", (char*)dp, "data2_on_nodes");
	return 0;
}
int Program::average( int d,int i, int j) {
	return s.dat_list[d]->data[i]+ s.dat_list[d]->data[j];
}

double  Program::advection_diffusion(double* f) {  //需要修改
	double fx;
	fx = *f-U*dt*(*(f+1)-*(f-1))/(2*h)+D*dt*(*(f + 1) + *(f - 1) -2*(*f))/(h*h);
	return fx;
}
int Program::loop(double (Program::*fx)(double* a), int e, int d) {
	int n = s.element_list[e]->size;
	double *f1 = (double*)s.dat_list[d]->data;
	double* pp = (double*)malloc(sizeof(double)*(n+2));
	memset(pp, 0, sizeof(double) * (n + 2));
	for (int i = 0; i < n; i++) {
		pp[i+1] = ((double*)(s.dat_list[d]->data))[i];
	}
	pp[0] = (double)s.dat_list[d]->data[0];
	pp[n+1] = (double)s.dat_list[d]->data[n-1];
	for (int i = 0; i < n; i++) {
		f1[i] = (this->*fx)(pp+i+1);
	}

	return 0;
}
int Program::program(int nstep, int e, int d) {  //需要修改
	double tm = 0;
	int n = s.element_list[e]->size;
	for (int j = 0; j < nstep; j++,tm+=dt) {
		loop(&Program::advection_diffusion,e,d);
	}
	return 0;
}