/*
 * Wavelet.h
 * Olivier Petit
 * CentraleSupelec
 * Hardware implementation of signal processing algorithms courses
 * March 2018
 */

#ifndef WAVELET_H_
#define WAVELET_H_

#include <systemc.h>

class Wavelet : public sc_module
{
public:
   sc_in<bool> clk;
   sc_in<bool> reset;
   
   // True if loading even pixel, false if loading odd pixel
   sc_in<bool> load_even;
   sc_in<unsigned char> data_in;

   // Data out, alternatively C_n and D_n
   sc_out<unsigned char> data_out;

   Wavelet(sc_module_name name);

   void delay();

   void compute_d();
   void compute_c();

   void output_select();

   SC_HAS_PROCESS(Wavelet);

   sc_signal<unsigned char> evenp4, evenp2, even, evenm2;
   sc_signal<unsigned char> oddp4, oddp2, odd;
   sc_signal<unsigned char> dm1;

   // Combinationnal computation of c and d
   // These are incorrect when we have loaded the even pixel but not the 
   // following odd pixel
   sc_signal<unsigned char> c_int;
   sc_signal<unsigned char> d_int;

   // Saves the last correct C value, so we can output it in between
   // even pixel loading and odd pixel loading
   sc_signal<unsigned char>  c_saved;

};
#endif