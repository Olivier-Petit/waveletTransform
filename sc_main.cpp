#include <systemc.h>
#include <stdio.h>

#include "WaveletProc.h"
#include "TestGene.h"

int sc_main(int argc, char *argv[])
{
	sc_clock clock("clock", 100, SC_NS, 1);
	sc_signal<bool> reset;
	sc_signal<bool> start;
	sc_signal<bool> busy;

	WaveletProc wp("Wavelet");

	wp.clk(clock);
	wp.reset(reset);
	wp.start(start);
	wp.busy(busy);

	TestGene tg("TestGene");
	tg.reset(reset);
	tg.start(start);

	sc_trace_file *tf;
	tf = sc_create_vcd_trace_file ("trace");
	sc_trace(tf, clock, "clock");
	sc_trace(tf, reset, "reset");
	sc_trace(tf, start, "start");
	sc_trace(tf, busy, "busy");

	cout << "Sim start" << endl;
	sc_start(540 * 512 * 100, SC_NS);
	sc_close_vcd_trace_file(tf);
	cout << "Sim end" << endl;

	return(0);
}