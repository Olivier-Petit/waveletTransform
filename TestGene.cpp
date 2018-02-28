#include "TestGene.h"

TestGene::TestGene(sc_module_name nm)
:sc_module(nm)
{
	SC_THREAD(genreset);
	SC_THREAD(genstart);
}

void TestGene::genreset()
{
	reset = false;
	wait(1,SC_US);
	reset = true;
	wait();
}

void TestGene::genstart()
{
	start = false;
	wait(1500, SC_NS);
	start = true;
	wait(200, SC_NS);
	start = false;
	wait();
}

