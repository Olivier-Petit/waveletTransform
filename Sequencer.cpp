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
		first_c.write(false);
		even.write(true);
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
		int addrIn = actualCol + nbCols * currentRow.read();
		mem_in_addr.write(addrIn);

		bool evenCol = actualCol % 2 == 0;
		even.write(evenCol);

		// Use C0 or Cj formula for C
		if(currentCol )
			first_c.write(true);
		else
			first_c.write(false);

		// Output address to access
		// Inputing even column, the transform blocks outputs D value
		if(evenCol)
		{	
			// The first D value is obtained when the even input pixel index is >= 6
			if(currentCol.read() >= 6)
			{
				mem_out_addr.write(
					nbCols * currentRow.read() // Position on right row
					+ (currentCol.read() - 6) / 2 // Column position
				);
				mem_out_write.write(true);
			}
			else
			{
				mem_out_addr.write(0);
				mem_out_write.write(false);
			}
		}
		else // Inputing odd column, the transform block outputs C value
		{
			if(currentCol.read() >= 7)
			{
				mem_out_addr.write(
					nbCols * currentRow.read()  // Position on right row
					+ nbCols / 2 // Offset by half the image
					+ (currentCol.read() - 7) / 2 // Column position
				);
				mem_out_write.write(true);
			}
			else
			{
				mem_out_addr.write(0);
				mem_out_write.write(false);
			}
		}

	}
}