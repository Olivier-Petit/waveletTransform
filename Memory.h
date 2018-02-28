#ifndef MEMORY_H_
#define MEMORY_H_

#include <systemc.h>
#include <stdio.h>


class Memory : public sc_module
{
public:
   sc_in<bool> clk;
   sc_in<bool> reset;
   
   sc_in<bool> img_valid;
   sc_in<int>  admemr;
   sc_in<int>  admemw;
   sc_in<unsigned char> data_bi, data_vi, data_ri;
   
   sc_out<unsigned char> data_bo, data_vo, data_ro;
   
   void pict_load();
   void pict_save();
   void mem_read();
   void mem_write();
      
   Memory(sc_module_name name);
   SC_HAS_PROCESS(Memory);
   ~Memory();

private:
   unsigned char *palb, *palv, *palr; // pointeurs vers les palettes couleurs
   unsigned char *imgb, *imgv, *imgr; // pointeurs vers les donnees image
   int *infimg;                       // pointeur sur les informations d'image
   int lgpal;                         // longueur de la palette
   int nbpix;                         // nombre de pixels dans l'image
   int nlig;                          // nombre de lignes de l'image
   int ncol;                          // nombre de colonnes de l'image
};
#endif