
#include "../inc/TestHarness.h"
#include "Mesh.h"
#include <iostream>
using namespace std;

int running(int argc, char* argv[]);

int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.³Â¶÷æÃ.Mesh10";  //Change to your ID and name
bool submission = false;  // set to true if you want to submit

LPCTSTR m_szFileName[20] = {
	"Program.cpp",
	"Mesh.h",
	"Mesh.cpp",
	"MeshProgramTest.cpp",
	""
};

#ifdef CodeExecutable

TEST(Meshr, creation, 9)
{
	//////////////////////////////////////////////////////////////// Test 9
//	Mesh s9;
//	Program1 prog1(s9);
//	prog1.run(0);
	/*
	double* p9; // = (double*)s9.dat_list[2]->data;
	double* p99;  // = (double*)s9.dat_list[5]->data;
	//the following check should be modified according to your results
	CHECK_DOUBLES_EQUAL(p9[0], 1, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[2], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[4], 1, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[6], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[8], 1, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[10], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[12], 1, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p9[14], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[170000], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17002], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17004], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17006], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17008], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17010], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17012], 0, 0.0000001, 10);
	CHECK_DOUBLES_EQUAL(p99[17014], 0, 0.0000001, 10);
	//CHECK(strcmp(s9.dat_list[0]->type, "int") == 0, 10);
	//CHECK(strcmp(s9.dat_list[1]->type, "double") == 0, 10);
	//CHECK(strcmp(s9.dat_list[2]->type, "double") == 0, 10);
	//CHECK(strcmp(s9.dat_list[3]->type, "double") == 0, 10);
*/

	int argc=3;
	char* argv[] = { {"mpiexec"},{"-n"},{"4"} };

	running(argc,argv);

}
#else
TEST(Meshr, creation, 9) {}
#endif

