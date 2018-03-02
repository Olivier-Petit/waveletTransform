/*
 * WaveletProc.cpp
 * Olivier Petit
 * CentraleSupelec
 * Hardware implementation of signal processing algorithms courses
 * March 2018
 */

#include "WaveletProc.h"

WaveletProc::WaveletProc(sc_module_name name):
	sc_module(name),
	sequencer("Sequencer"),
	memory("Memory"),
	waveletR("WaveletR"),
	waveletG("WaveletG"),
	waveletB("WaveletB")
{
	// Sequencer
	sequencer.clk(clk);
	sequencer.reset(reset);
	sequencer.start(start);
	sequencer.iterations(iterations);
	sequencer.busy(busy);
	sequencer.mem_in_addr(mem_in_addr);
	sequencer.mem_out_addr(mem_out_addr);
	sequencer.mem_out_write(mem_out_write);
	sequencer.even(even);
	sequencer.first_c(first_c);
	sequencer.sizeX(sizeX);
	sequencer.sizeY(sizeY);

	// Memory
	memory.clk(clk);
	memory.reset(reset);
	memory.img_valid(busy);
	memory.admemr(mem_in_addr);
	memory.admemw(mem_out_addr);
	memory.write_enable(mem_out_write);
	memory.data_bi(data_bi);
	memory.data_vi(data_gi);
	memory.data_ri(data_ri);
	memory.data_bo(data_bo);
	memory.data_vo(data_go);
	memory.data_ro(data_ro);
	memory.sizeX(sizeX);
	memory.sizeY(sizeY);

	// Wavelet
	waveletR.clk(clk);
	waveletR.reset(reset);
	waveletR.load_even(even);
	waveletR.first_c(first_c);
	waveletR.data_in(data_ro);
	waveletR.data_out(data_ri);

	waveletG.clk(clk);
	waveletG.reset(reset);
	waveletG.load_even(even);
	waveletG.first_c(first_c);
	waveletG.data_in(data_go);
	waveletG.data_out(data_gi);

	waveletB.clk(clk);
	waveletB.reset(reset);
	waveletB.load_even(even);
	waveletB.first_c(first_c);
	waveletB.data_in(data_bo);
	waveletB.data_out(data_bi);
}