#include "WaveletProc.h"

WaveletProc::WaveletProc(sc_module_name name):
	sc_module(name),
	sequencer("Sequencer", 512, 512),
	memory("Memory"),
	waveletR("WaveletR"),
	waveletG("WaveletG"),
	waveletB("WaveletB")
{

}