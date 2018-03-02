/*
 * Sequencer.h
 * Olivier Petit
 * CentraleSupelec
 * Hardware implementation of signal processing algorithms courses
 * March 2018
 */

#ifndef SEQUENCER_H_
#define SEQUENCER_H_

#include <systemc.h>

class Sequencer : public sc_module
{
public:
	sc_in<bool> clk;
	sc_in<bool> reset;

	// Sequencer control signals
	sc_in<bool> start;
	sc_in<int> iterations;
	sc_out<bool> busy;

	// Input image size
	sc_in<int> sizeX;
	sc_in<int> sizeY;

	// Memory control signals
	sc_out<int> mem_in_addr;
	sc_out<int> mem_out_addr;
	sc_out<bool> mem_out_write;

	// Transform block control signals
	sc_out<bool> even;
	sc_out<bool> first_c;

	Sequencer(sc_module_name name);

	void update_state();
	void compute_outputs();

	SC_HAS_PROCESS(Sequencer);

private:
	sc_signal<int> globalState;
	sc_signal<int> currentRow;
	sc_signal<int> currentCol;
	sc_signal<int> remainingRuns;

	int nbRows;
	int nbCols;
};

#endif