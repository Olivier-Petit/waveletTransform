#include "Sequencer.h"

#define SEQ_STATE_READY 0
#define SEQ_STATE_BUSY 1

Sequencer::Sequencer(sc_module_name name, int nbRows, int nbCols) : 
	sc_module(name), nbRows(nbRows), nbCols(nbCols)
{	
	// Sequencer logic is falling edge triggered
	SC_METHOD(update_state);
		sensitive << clk.neg() << reset;

	SC_METHOD(compute_outputs);
		sensitive << globalState << currentRow << currentCol;
}

void Sequencer::update_state()
{
	// Reset
	if(reset.read() == false)
	{
		globalState.write(SEQ_STATE_READY);
		busy.write(false);
	}
	// Start transform
	else if(globalState.read() == SEQ_STATE_READY && start.read() == true)
	{
		globalState.write(SEQ_STATE_BUSY);
		currentRow.write(0);
		currentCol.write(-2);
	}
	// Computing
	else if(globalState.read() == SEQ_STATE_BUSY)
	{
		currentCol.write(currentCol.read() + 1);

		// Row over, switch to next one
		if(currentCol.read() == nbCols + 2)
		{
			currentRow.write(currentRow.read() + 1);
			currentCol.write(-2);
			
			// Image over
			if(currentRow.read() == nbRows - 1)
			{
				globalState.write(SEQ_STATE_READY);
			}
		}
	}
}

void Sequencer::compute_outputs()
{
	if(globalState.read() == SEQ_STATE_READY)
	{
		busy.write(false);
		mem_in_addr.write(0);
		mem_out_addr.write(0);
		mem_out_write.write(false);
	}
	else // Computing
	{
		busy.write(true);

		// Manage image folding on the sides
		int actualCol = currentCol.read();
		if(actualCol < 0)
			actualCol = -actualCol;
		else if(actualCol > nbCols)
			actualCol = nbCols - (actualCol - nbCols);

		// Input address to access
		int addrIn = actualCol + nbRows * currentRow.read();
		mem_in_addr.write(addrIn);

		if(actualCol % 2 == 0)
		{
			load_even.write(true);
			load_odd.write(false);
		}
		else
		{
			load_even.write(false);
			load_odd.write(true);
		}
	}
}