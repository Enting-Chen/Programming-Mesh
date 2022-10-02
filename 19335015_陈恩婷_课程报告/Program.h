#pragma once
class Mesh;
class Program
{
	double dt, h, U, D;
	Mesh s;
public:
	Program(Mesh &s);
	Program(Mesh &s,double dt, double h, double U, double D);
	~Program();
	int init();
	int average(int d, int a,int b);
	double advection_diffusion(double *f);  //kernel function

	int loop(double (Program::*fx)(double* a), int e, int d);
	int program(int nstep, int e, int d);
};

