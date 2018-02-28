#include "Wavelet.h"

Wavelet::Wavelet(sc_module_name name):sc_module(name)
{
	SC_METHOD(delay);
        sensitive << clk.pos() << reset;
    
    SC_METHOD(compute_d)
    	sensitive << evenm2 << even << evenp2 << evenp4 << odd << reset;
    
    SC_METHOD(compute_c)
    	sensitive << even << dm1 << d_int << first_c << reset;

    SC_METHOD(output_select);
    	sensitive << c_int << d_int << c_saved << load_even << reset;
}

void Wavelet::delay()
{
	if(reset.read() == false)
	{
		// Even delay line
		dm1.write(0);
		evenm2.write(0);
		even.write(0);
		evenp2.write(0);
		evenp4.write(0);

		// Odd delay line
		odd.write(0);
		oddp2.write(0);
		oddp4.write(0);
	}
	// Load new even pixel
	else if(load_even.read() == true)
	{
		// Save old D value at this point
		dm1.write(d_int.read());

		// Go 1 step forward
		evenm2.write(even.read());
		even.write(evenp2.read());
		evenp2.write(evenp4.read());
		evenp4.write(data_in.read());

		// Save valid c value, so we can output it later
		c_saved.write(c_int.read());
	}
	// Load new odd pixel
	else if(load_even.read() == false)
	{
		odd.write(oddp2.read());
		oddp2.write(oddp4.read());
		oddp4.write(data_in.read());
	}
}

void Wavelet::compute_d()
{
	if(reset.read() == false)
	{
		d_int.write(0);
	}
	else
	{
		int newd = 
			odd.read() 
			- (9*(even.read() + evenp2.read()) 
				- (evenm2.read() + evenp4.read())) / 16
			+ 128;

		if(newd > 255)
			d_int.write(255);
		else if(newd < 0)
			d_int.write(0);
		else
			d_int.write(newd);
	}
}

void Wavelet::compute_c()
{
	if(reset.read() == false)
	{
		c_int.write(0);
	}
	else
	{
		int newc = 0;
		if(first_c.read() == true)
			newc = even.read() + d_int.read() / 2 - 64;
		else
			newc = even.read() + (d_int.read() + dm1.read()) / 4 - 64;

		if(newc > 255)
			c_int.write(255);
		else if(newc < 0)
			c_int.write(0);
		else
			c_int.write(newc);
	}
}

void Wavelet::output_select()
{
	if(reset.read() == false)
	{
		data_out.write(0);
	}
	else
	{
		if(load_even.read() == true)
			data_out.write(d_int.read());
		else
			data_out.write(c_saved.read());
	}
}