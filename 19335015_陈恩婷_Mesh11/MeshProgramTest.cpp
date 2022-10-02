
#include "../inc/TestHarness.h"
#include "Mesh.h"
#include <iostream>
using namespace std;

int running(int argc, char* argv[]);

int TestNum = 0;    //Change this definition (0,1,2,5)
string YourName = "19335015.³Â¶÷æÃ.Mesh11";  //Change to your ID and name
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
	int argc=3;
	char* argv[] = { {"mpiexec"},{"-n"},{"4"} };

	running(argc,argv);

}
#else
TEST(Meshr, creation, 9) {}
#endif

