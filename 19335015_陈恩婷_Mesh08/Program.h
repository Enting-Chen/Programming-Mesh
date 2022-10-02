#pragma once
class Mesh;
class Program
{
	double dt, h, U, D;
	Mesh & s;
public:
	Program(Mesh & s);
	Program(Mesh & s,double dt, double h, double U, double D);
	~Program();
	int init();
	int average(int d, int a,int b); //没有用到
	double advection_diffusion(double *f);  //kernel function

	int loop(double (Program::*fx)(double* a), int element, int data);
	// 把fx在数组上操作一遍
	int program(int nstep, int e, int d);
};

