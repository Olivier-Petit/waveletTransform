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
	sc_signal<int> iterations;
	iterations.write(1);

	WaveletProc wp("Wavelet");

	wp.clk(clock);
	wp.reset(reset);
	wp.start(start);
	wp.iterations(iterations);
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
	sc_trace(tf, wp.even, "even");
	sc_trace(tf, wp.first_c, "first_c");
	sc_trace(tf, wp.mem_out_write, "mem_out_write");
	sc_trace(tf, wp.mem_in_addr, "mem_in_addr");
	sc_trace(tf, wp.mem_out_addr, "mem_out_addr");
	sc_trace(tf, wp.waveletR.data_out, "r_out");
	sc_trace(tf, wp.waveletR.c_int, "r_c");
	sc_trace(tf, wp.waveletR.d_int, "r_d");

	cout << "Sim start" << endl;
	sc_start(540 * 512 * 200 * 2, SC_NS);
	sc_close_vcd_trace_file(tf);
	cout << "Sim end" << endl;

	return(0);
}