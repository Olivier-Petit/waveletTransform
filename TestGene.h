/*
 * TestGene.h
 * Olivier Petit
 * CentraleSupelec
 * Hardware implementation of signal processing algorithms courses
 * March 2018
 */

#ifndef TEST_GENE_H
#define TEST_GENE_H

#include <systemc.h>

SC_MODULE(TestGene)
{	
	sc_out<bool> reset;
	sc_out<bool> start;

	void genreset();
	void genstart();

	TestGene(sc_module_name nm);
	SC_HAS_PROCESS(TestGene);
};

#endif
