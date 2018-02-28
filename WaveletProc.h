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
	sc_in<bool> busy;

	WaveletProc(sc_module_name name);

	Sequencer sequencer;
	Memory memory;
	Wavelet waveletR;
	Wavelet waveletG;
	Wavelet waveletB;
};

#endif