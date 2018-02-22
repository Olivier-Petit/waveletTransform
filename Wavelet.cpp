#include "Wavelet.h"

Wavelet::Wavelet(sc_module_name name):sc_module(name)
{
	SC_METHOD(delay_even);
        sensitive << clk.pos() << reset;
    SC_METHOD(delay_odd);
    	sensitive << clk.pos() << reset;
    SC_METHOD(compute_d)
    	sensitive << evenm2 << even << evenp2 << evenp4 << odd << reset;
    SC_METHOD(compute_c)
    	sensitive << even << dm1 << d_out << first_c << reset;
}

void Wavelet::delay_even()
{
	if(reset.read() == false)
	{
		dm1.write(0);
		evenm2.write(0);
		even.write(0);
		evenp2.write(0);
		evenp4.write(0);
	}
	else if(load_even.read() == true)
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
	if(reset.read() == false)
	{
		odd.write(0);
		oddp2.write(0);
		oddp4.write(0);
	}
	else if(load_odd.read() == true)
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
		d_out.write(0);
	}
	else
	{
		d_out.write(
			odd.read() 
			- (9*(even.read() + evenp2.read()) 
				- (evenm2.read() + evenp4.read())) / 16
			-128 
		);
	}
}

void Wavelet::compute_c()
{
	if(reset.read() == false)
	{
		c_out.write(0);
	}
	else
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
}