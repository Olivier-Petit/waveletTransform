#ifndef WAVELET_PROC_H
#define WAVELET_PROC_H

#include <systemc.h>

#include "Memory.h"
#include "Sequencer.h"
#include "Wavelet.h"

class WaveletProc : public sc_module
{
public:
	sc_in<bool> clk;
	sc_in<bool> reset;

	sc_in<bool> start;
	sc_out<bool> busy;

	WaveletProc(sc_module_name name);

	Sequencer sequencer;
	Memory memory;
	Wavelet waveletR;
	Wavelet waveletG;
	Wavelet waveletB;

	sc_signal<int> sizeX, sizeY;

	sc_signal<int> mem_in_addr;
	sc_signal<int> mem_out_addr;
	sc_signal<bool> mem_out_write;
	sc_signal<bool> even;
	sc_signal<bool> first_c;

	sc_signal<unsigned char> data_bi, data_gi, data_ri;
   	sc_signal<unsigned char> data_bo, data_go, data_ro;
};

#endif