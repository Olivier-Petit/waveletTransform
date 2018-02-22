#include "Wavelet.h"

Wavelet::Wavelet(sc_module_name name):sc_module(name)
{
	SC_METHOD(delay_even);
        sensitive << clk.pos();
    SC_METHOD(delay_odd);
    	sensitive << clk.pos();
    SC_METHOD(compute_d)
    	sensitive << evenm2 << even << evenp2 << evenp4 << odd;
    SC_METHOD(compute_c)
    	sensitive << even << dm1 << d_out << first_c;
}

void Wavelet::delay_even()
{
	if(load_even.read() == true)
	{
		// Save old D value at this point
		dm1.write(d_out.read());

		// Go 1 step forward
		evenm2.write(even.read());
		even.write(evenp2.read());
		evenp2.write(evenp4.read());
		evenp4.write(data_in.read());
	}
}

void Wavelet::delay_odd()
{
	if(load_odd.read() == true)
	{
		odd.write(oddp2.read());
		oddp2.write(oddp4.read());
		oddp4.write(data_in.read());
	}
}

void Wavelet::compute_d()
{
	d_out.write(
		odd.read() 
		- (9*(even.read() + evenp2.read()) 
			- (evenm2.read() + evenp4.read())) / 16
		-128 
	);
}

void Wavelet::compute_c()
{
	if(first_c.read() == true)
	{
		c_out.write(even.read() + d_out.read() / 2);
	}
	else
	{
		c_out.write(even.read() + (d_out.read() + dm1.read()) / 4);
	}
}