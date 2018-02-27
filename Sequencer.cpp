#include "Sequencer.h"

Sequencer::Sequencer(sc_module_name name) : sc_module(name)
{	
	// Sequencer logic is falling edge triggered
	SC_METHOD(update_state);
		sensitive << clk.neg() << reset;

	SC_METHOD(compute_outputs);
		sensitive << globalState << currentLine << currentCol;
}

void Sequencer::update_state()
{

}

void Sequencer::compute_outputs()
{

}