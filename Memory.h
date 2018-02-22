#ifndef MEMORY_H_
#define MEMORY_H_

#include <systemc.h>
#include <stdio.h>
//#include "defs.h"


SC_MODULE(Memory) {
   ////////////////////////////////////////
   // definition des variables locales
   ///////////////////////////////////////
   
   ////////////////////////////////////////
   // definition des ports d'entrée sortie
   ///////////////////////////////////////
   sc_in<bool> clk;
   sc_in<bool> reset;
   
   sc_in<bool> img_valid;
   sc_in<int>  admemr;
   sc_in<int>  admemw;
   sc_in<unsigned char> data_bi, data_vi, data_ri;
   
   sc_out<unsigned char> data_bo, data_vo, data_ro;
   
   ///////////////////////////////////////////////
   // definition des fonctions locales (processus)
   ///////////////////////////////////////////////
   void pict_load();
   void pict_save();
   void mem_read();
   void mem_write();
      
   ///////////////////////////////////////////////
   // constructeur C++
   ///////////////////////////////////////////////
   Memory(sc_module_name name);
   SC_HAS_PROCESS(Memory);
   ~Memory();
};
#endif //include guard