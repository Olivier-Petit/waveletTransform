#ifndef WAVELET_H_
#define WAVELET_H_

#include <systemc.h>

class Wavelet : public sc_module
{
public:
   sc_in<bool> clk;
   sc_in<bool> reset;
   
   sc_in<bool> load_even;
   sc_in<bool> load_odd;
   sc_in<bool> first_c;
   sc_in<unsigned char> data_in;
   
   sc_out<unsigned char> c_out;
   sc_out<unsigned char> d_out;

   Wavelet(sc_module_name name);

   void delay_even();
   void delay_odd();

   void compute_d();
   void compute_c();

   SC_HAS_PROCESS(Wavelet);

private:
   sc_signal<unsigned char> evenp4, evenp2, even, evenm2;
   sc_signal<unsigned char> oddp4, oddp2, odd;
   sc_signal<unsigned char> dm1;
};
#endif