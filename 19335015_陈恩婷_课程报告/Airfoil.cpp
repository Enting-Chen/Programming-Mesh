
#include "Mesh.h"
#include <mpi.h>
#include <string>
using namespace std;

#ifdef CodeExecutable

double gam, gm1, cfl, eps, mach, alpha, qinf[4];  //7 constants
int tag = 0;

void initflow() {
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

	return;
}


//kernels

void save(const double* q, double* qold) {
	for (int n = 0; n < 4; n++) qold[n] = q[n];//æ¯æ?¡åªå­˜äº†å››ä¸ª
}

void area(const double* x1, const double* x2, const double* x3,
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

void flux(const double* x1, const double* x2, const double* q1,
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

void bcond(const double* x1, const double* x2, const double* q1,
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

void update(const double* qold, double* q, double* res,
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
//
// user declared functions
//

static int compute_local_size(int global_size, int mpi_comm_size,
	int mpi_rank) {
	int local_size = global_size / mpi_comm_size;
	int remainder = (int)fmod(global_size, mpi_comm_size);

	if (mpi_rank < remainder) {
		local_size = local_size + 1;
	}
	return local_size;
}

static void scatter_double_array(double* g_array, double* l_array,
	int comm_size, int g_size, int l_size,
	int elem_size) {
	int* sendcnts = (int*)malloc(comm_size * sizeof(int));
	int* displs = (int*)malloc(comm_size * sizeof(int));
	int disp = 0;

	for (int i = 0; i < comm_size; i++) {
		sendcnts[i] = elem_size * compute_local_size(g_size, comm_size, i);
	}
	for (int i = 0; i < comm_size; i++) {
		displs[i] = disp;
		disp = disp + sendcnts[i];
	}

	MPI_Scatterv(g_array, sendcnts, displs, MPI_DOUBLE, l_array,
		l_size * elem_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	free(sendcnts);
	free(displs);
}

static void scatter_int_array(int* g_array, int* l_array, int comm_size,
	int g_size, int l_size, int elem_size) {
	int* sendcnts = (int*)malloc(comm_size * sizeof(int));
	int* displs = (int*)malloc(comm_size * sizeof(int));
	int disp = 0;

	for (int i = 0; i < comm_size; i++) {
		sendcnts[i] = elem_size * compute_local_size(g_size, comm_size, i);
	}
	for (int i = 0; i < comm_size; i++) {
		displs[i] = disp;
		disp = disp + sendcnts[i];
	}

	MPI_Scatterv(g_array, sendcnts, displs, MPI_INT, l_array, l_size * elem_size,
		MPI_INT, 0, MPI_COMM_WORLD);

	free(sendcnts);
	free(displs);
}

static void gather_double_array(double* g_array, double* l_array,
	int comm_size, int g_size, int l_size,
	int elem_size) {
	int sendcnt, i;
	MPI_Comm_rank(MPI_COMM_WORLD, &i);
	int* sendcnts = (int*)malloc(comm_size * sizeof(int));
	int* displs = (int*)malloc(comm_size * sizeof(int));
	int disp = 0;

	for (int i = 0; i < comm_size; i++) {
		sendcnts[i] = elem_size * compute_local_size(g_size, comm_size, i);
	}
	for (int i = 0; i < comm_size; i++) {
		displs[i] = disp;
		disp = disp + sendcnts[i];
	}

	MPI_Gatherv(l_array, sendcnts[i], MPI_DOUBLE, g_array + displs[i],
			sendcnts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	free(sendcnts);
	free(displs);
}

static void gather_int_array(int* g_array, int* l_array, int comm_size,
	int g_size, int l_size, int elem_size) {
	int* sendcnts = (int*)malloc(comm_size * sizeof(int));
	int* displs = (int*)malloc(comm_size * sizeof(int));
	int disp = 0;

	for (int i = 0; i < comm_size; i++) {
		sendcnts[i] = elem_size * compute_local_size(g_size, comm_size, i);
	}
	for (int i = 0; i < comm_size; i++) {
		displs[i] = disp;
		disp = disp + sendcnts[i];
	}

//	MPI_Gatherv(g_array, sendcnts, displs, MPI_INT, l_array, l_size * elem_size,
//		MPI_INT, 0, MPI_COMM_WORLD);

	free(sendcnts);
	free(displs);
}

static void check_scan(int items_received, int items_expected) {
	if (items_received != items_expected) {
		printf("error reading from new_grid.dat\n");
		exit(-1);
	}
}

static void compute_local_range(int global_size, int mpi_comm_size,
	int mpi_rank, int& begin, int& end) {
	begin = 0;
	for (int i = 0; i < mpi_rank; i++) {
		begin += compute_local_size(global_size, mpi_comm_size, i);
	}
	end = begin + compute_local_size(global_size, mpi_comm_size, mpi_rank);
}

static void compute_local_range(int global_size, int mpi_comm_size,
	int mpi_rank, int& begin, int& end) {
	begin = 0;
	for (int i = 0; i < mpi_rank; i++) {
		begin += compute_local_size(global_size, mpi_comm_size, i);
	}
	end = begin + compute_local_size(global_size, mpi_comm_size, mpi_rank);
}

int calculate_source_rank(int mpi_comm_size, int g_size, int g_begin, int elem_size) {
	int* sendcnts = (int*)malloc(mpi_comm_size * sizeof(int));
	int* displs = (int*)malloc(mpi_comm_size * sizeof(int));

	int disp = 0;
	for (int i = 0; i < mpi_comm_size; i++) {
		sendcnts[i] = elem_size * compute_local_size(g_size, mpi_comm_size, i);
	}
	for (int i = 0; i < mpi_comm_size; i++) {
		displs[i] = disp;
		disp = disp + sendcnts[i];
	}

	// find which process has the data
	int source_rank = 0;
	for (int i = 0; i < mpi_comm_size; i++) {
		if (g_begin < displs[i] + sendcnts[i]) {
			source_rank = i;
			break;
		}
	}
	return source_rank;
}

void get_int_array_from_another_process(int g_size, int l_size, int elem_size,
	int my_rank, int destination_rank, int mpi_comm_size, int g_begin, int* data, int* g_array) {
	int source_rank = calculate_source_rank(mpi_comm_size, g_size, g_begin, elem_size);

	// calculate local l_begin
	int l_begin = 0;
	for (int i = 0; i < source_rank; i++) {
		l_begin += compute_local_size(g_size, mpi_comm_size, i);
	}

	// check if the data is in the same process
	if (source_rank == destination_rank) {
		// data is in the same process
		// load data into array
		if (source_rank == my_rank) {
			for (int i = 0; i < l_size; i++) {
				for (int j = 0; j < elem_size; j++) {
					data[i * elem_size + j] = g_array[(g_begin + i) * elem_size + j];
				}
			}
		}
		else {
			//do nothing
		}
	}
	else {
		// data is in another process
		// send data to the process 
		if (my_rank == source_rank) {
			MPI_Send(g_array + g_begin * elem_size, l_size * elem_size, MPI_INT, destination_rank, tag, MPI_COMM_WORLD);
		}
		else if (my_rank == destination_rank) {
			MPI_Recv(data, l_size * elem_size, MPI_INT, source_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		else {
			//do nothing
		}
		tag++;
	}
}

void get_double_array_from_another_process(int g_size, int l_size, int elem_size,
	int my_rank, int destination_rank, int mpi_comm_size, int g_begin, double* data, double* g_array) {
	int source_rank = calculate_source_rank(mpi_comm_size, g_size, g_begin, elem_size);

	// calculate local l_begin
	int l_begin = 0;
	for (int i = 0; i < source_rank; i++) {
		l_begin += compute_local_size(g_size, mpi_comm_size, i);
	}

	// check if the data is in the same process
	if (source_rank == destination_rank) {
		// data is in the same process
		// load data into array
		if (source_rank == my_rank) {
			for (int i = 0; i < l_size; i++) {
				for (int j = 0; j < elem_size; j++) {
					data[i * elem_size + j] = g_array[(g_begin + i) * elem_size + j];
				}
			}
		}
	}
	else {
		// data is in another process
		// send data to the process 
		if (my_rank == source_rank) {
			MPI_Send(g_array + g_begin * elem_size, l_size * elem_size, MPI_DOUBLE, destination_rank, tag, MPI_COMM_WORLD);
		}
		else if (my_rank == destination_rank) {
			MPI_Recv(data, l_size * elem_size, MPI_DOUBLE, source_rank, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		tag++;
	}
}


int partitionTo(int* cells, double* p_x)
{
	return 0;
}

int partitionFrom(int* cells, double* p_x)
{
	return 0;
}


//The following program is logically not correct, just for demo
int running(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int my_rank, namelen, comm_size;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Get_processor_name(processor_name, &namelen);

	//Global Mesh 
	int g_nnode = 0, g_ncell = 0, g_nedge = 0, g_nbedge = 0;
	int* g_becell = 0, * g_ecell = 0, * g_bound = 0, * g_bedge = 0, * g_edge = 0,
		* g_cell = 0;
	double* g_x = 0, * g_q = 0, * g_qold = 0, * g_adt = 0, * g_res = 0;

	//local mesh
	int nnode, ncell, nedge, nbedge;
	int* becell, * ecell, * bound, * bedge, * edge, * cell;
	double* x, * q, * qold, * adt, * res;
	double rms, g_rms;

	initflow();//

	FILE* fp;

	if ((fp = fopen("./new_grid.dat", "r")) == NULL) {
		printf("can't open file new_grid.dat\n");
		exit(-1);
	}

	readrawHeader(fp, &g_nnode, &g_ncell, &g_nedge, &g_nbedge);

	//if (my_rank == 0) {
	g_cell = (int*)malloc(4 * g_ncell * sizeof(int));	//ä¸€ä¸ªcell 4ä¸?ç‚?
	g_edge = (int*)malloc(2 * g_nedge * sizeof(int));	//ä¸€æ¡è¾¹ 2ä¸?ç‚?
	g_ecell = (int*)malloc(2 * g_nedge * sizeof(int));	//ä¸€ä¸ªecell 2ä¸?ç‚?
	g_bedge = (int*)malloc(2 * g_nbedge * sizeof(int));	//ä¸€æ?bè¾?2ä¸?ç‚?
	g_becell = (int*)malloc(g_nbedge * sizeof(int));	//ä¸€ä¸ªbecell 1ä¸ªcell

	g_x = (double*)malloc(3 * g_nnode * sizeof(double));//ä¸€ä¸?ç‚¹ä¸‰ä¸?åæ ‡
	g_q = (double*)malloc(4 * g_ncell * sizeof(double));
	g_qold = (double*)malloc(4 * g_ncell * sizeof(double));
	g_res = (double*)malloc(4 * g_ncell * sizeof(double));
	g_adt = (double*)malloc(g_ncell * sizeof(double));
	g_bound = (int*)malloc(g_nbedge * sizeof(int));

	readrawNode(fp, g_nnode, g_x);
	readrawCell(fp, g_ncell, g_cell);
	readrawEdge(fp, g_nedge, g_edge, g_ecell);
	readrawBEdge(fp, g_nbedge, g_bedge, g_becell, g_bound);

	for (int n = 0; n < g_ncell; n++) { //ncell
		for (int m = 0; m < 4; m++) {
			g_q[4 * n + m] = qinf[m];
			g_res[4 * n + m] = 0.0f;
		}
	}

	//}//my_rank == 0

	fclose(fp);

	MPI_Barrier(MPI_COMM_WORLD);


	nnode = compute_local_size(g_nnode, comm_size, my_rank);
	ncell = compute_local_size(g_ncell, comm_size, my_rank);
	nedge = compute_local_size(g_nedge, comm_size, my_rank);
	nbedge = compute_local_size(g_nbedge, comm_size, my_rank);

	fprintf(stdout, "Number of nodes, cells, edges, bedges on process %d = %d, %d, %d, %d\n",
		my_rank, nnode, ncell, nedge, nbedge);
	fflush(stdout);

	if (my_rank == 0) {
		partitionTo(g_cell, g_x);
		partitionFrom(g_edge, g_x);
		partitionFrom(g_bedge, g_x);
	}

	/*Allocate memory to hold local sets, mapping tables and data*/
	cell = (int*)malloc(4 * ncell * sizeof(int));
	edge = (int*)malloc(2 * nedge * sizeof(int));
	ecell = (int*)malloc(2 * nedge * sizeof(int));
	bedge = (int*)malloc(2 * nbedge * sizeof(int));
	becell = (int*)malloc(nbedge * sizeof(int));
	bound = (int*)malloc(nbedge * sizeof(int));

	x = (double*)malloc(3 * nnode * sizeof(double));
	q = (double*)malloc(4 * ncell * sizeof(double));
	qold = (double*)malloc(4 * ncell * sizeof(double));
	res = (double*)malloc(4 * ncell * sizeof(double));
	adt = (double*)malloc(ncell * sizeof(double));

	/* scatter sets, mappings and data on sets*/
	scatter_int_array(g_cell, cell, comm_size, g_ncell, ncell, 4);
	scatter_int_array(g_edge, edge, comm_size, g_nedge, nedge, 2);
	scatter_int_array(g_ecell, ecell, comm_size, g_nedge, nedge, 2);
	scatter_int_array(g_bedge, bedge, comm_size, g_nbedge, nbedge, 2);
	scatter_int_array(g_becell, becell, comm_size, g_nbedge, nbedge, 1);
	scatter_int_array(g_bound, bound, comm_size, g_nbedge, nbedge, 1);

	scatter_double_array(g_x, x, comm_size, g_nnode, nnode, 3);
	scatter_double_array(g_q, q, comm_size, g_ncell, ncell, 4);
	scatter_double_array(g_qold, qold, comm_size, g_ncell, ncell, 4);
	scatter_double_array(g_res, res, comm_size, g_ncell, ncell, 4);
	scatter_double_array(g_adt, adt, comm_size, g_ncell, ncell, 1);

	if (my_rank == 0) {
		WriteMeshToVTKAscii("new_grid.vtk", g_x, g_nnode, g_cell, g_ncell, g_q);  //save total grid to vtk
	// 	free(g_cell);
	// 	free(g_edge);
	// 	free(g_ecell);
	// 	free(g_bedge);
	// 	free(g_becell);
	// 	free(g_bound);
	// 	free(g_x);
	// 	free(g_q);
	// 	free(g_qold);
	// 	free(g_adt);
	// 	free(g_res);
	}

	//save vtk for each process
	string filename = "new_grid_raw_" + to_string(my_rank) + ".dat";
	WriteRaw(filename.c_str(), nnode, ncell, nedge, nbedge, x, cell, edge, ecell, bedge, becell, bound, q);

	// free memory
	free(cell);
	free(edge);
	free(ecell);
	free(bedge);
	free(becell);
	free(bound);
	free(x);
	free(q);
	// test read and rewrite raw
	ReadRaw(filename.c_str(), &nnode, &ncell, &nedge, &nbedge, &x, &cell, &edge, &ecell, &bedge, &becell, &bound, &q);
	filename = "new_grid_raw_" + to_string(my_rank) + "_1.dat";
	WriteRaw(filename.c_str(), nnode, ncell, nedge, nbedge, x, cell, edge, ecell, bedge, becell, bound, q);

	// test read and write ascii emd
	Mesh s;
	Elements enodes = s.makeElements(nnode, "set_nodes");
	Elements ecells = s.makeElements(ncell, "set_cells");

	Map cell2point = s.makeMap(ecells, enodes, 4, cell, "cell_to_node_map");
	Map ecell2point = s.makeMap(ecells, enodes, 2, ecell, "ecell_to_node_map");
	Map becell2point = s.makeMap(ecells, enodes, 1, becell, "becell_to_node_map");
	Map edge2point = s.makeMap(ecells, enodes, 2, edge, "edge_to_node_map");
	Map bedge2point = s.makeMap(ecells, enodes, 2, bedge, "bedge_to_node_map");
	Map bound2point = s.makeMap(ecells, enodes, 1, bound, "bound_to_node_map");

	Data x_data = s.makeData(enodes, 3, "double", (char*)x, "x");

	filename = "new_grid_ascii_" + to_string(my_rank) + ".emd";
	s.WriteMeshToEMDAscii(filename.c_str());
	s.ReadMeshFromEMDAscii(filename.c_str());
	filename = "new_grid_ascii_" + to_string(my_rank) + "_1.emd";
	s.WriteMeshToEMDAscii(filename.c_str());

	if (my_rank == 0 ) {

	int niter = 100;

	rms = 0.0;
	for (int iter = 1; iter <= niter; iter++) {
		for (int i = 0; i < g_ncell; i++) {
			save(g_q + 4 * i, g_qold + 4 * i);
		}
		//MPI_Barrier(MPI_COMM_WORLD);

		//for (int k = 0; k < 2; k++) {
			for (int i = 0; i < g_ncell; i++) { //////////////
				area(
					g_x + (g_cell[4 * i]) * 3,
					g_x + (g_cell[4 * i + 1]) * 3,
					g_x + (g_cell[4 * i + 2]) * 3,
					g_x + (g_cell[4 * i + 3]) * 3,
					g_q + 4 * i,
					g_adt + i);
			}
			//MPI_Barrier(MPI_COMM_WORLD);

			for (int i = 0; i < g_nedge; i++) { //////////////
				flux(
					g_x + (g_edge[2 * i]) * 3,
					g_x + (g_edge[2 * i + 1]) * 3,
					g_q + (g_ecell[2 * i]) * 4,
					g_q + (g_ecell[2 * i + 1]) * 4,
					g_adt + (g_ecell[2 * i]),
					g_adt + (g_ecell[2 * i + 1]),
					g_res + (g_ecell[2 * i]) * 4,
					g_res + (g_ecell[2 * i + 1]) * 4);
			}
			//MPI_Barrier(MPI_COMM_WORLD);

			for (int i = 0; i < g_nbedge; i++) {  //////////////////
				bcond(
					g_x + (g_bedge[2 * i]) * 3,
					g_x + (g_bedge[2 * i + 1]) * 3,
					g_q + (g_becell[i]) * 4,
					g_adt + (g_becell[i]),
					g_res + (g_becell[i]) * 4,
					g_bound + i);
			}
			rms = 0.0;
			//MPI_Barrier(MPI_COMM_WORLD);

			for (int i = 0; i < g_ncell; i++) {
				update(
					g_qold + 4 * i,
					g_q + 4 * i,
					g_res + 4 * i,
					g_adt + i,
					&rms);
			}
		//}
		//MPI_Barrier(MPI_COMM_WORLD);
		rms = sqrt(rms / (double)g_ncell);
		if (iter % 10 == 0) {
			printf(" %d  %10.5e \n", iter, rms);
			//sprintf(buf, "%dout%d.vtk", my_rank,iter);
			//WriteMeshToVTKAscii(buf, x, nnode, cell, ncell, q);
		}
	}

	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	free(cell);
	free(edge);
	free(ecell);
	free(bedge);
	free(becell);
	free(bound);
	free(x);
	free(q);
	free(qold);
	free(res);
	free(adt);

	free(g_cell);
	free(g_edge);
	free(g_ecell);
	free(g_bedge);
	free(g_becell);
	free(g_bound);
	free(g_x);
	free(g_q);
	free(g_qold);
	free(g_adt);
	free(g_res);

	return 0;
}

#endif