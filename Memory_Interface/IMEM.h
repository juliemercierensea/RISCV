#include <systemc.h>
#ifndef IMEM_H
#define IMEM_H
#include "IHex.h"
#include "IHex.cpp"

SC_MODULE (scIMEM) {
    sc_lv<32> contents [256];

    typedef scIMEM SC_CURRENT_USER_MODULE;                          \
    scIMEM(const char *fname, ::sc_core::sc_module_name = "")
    {
        //SC_THREAD(state);
        IHex::IHexFile ihexfile(fname);
        ihexfile.exportSystemC(0,256,contents);
    }

    /*void state() {
        while (1){
            ihexfile.exportSystemC(0, 256, contents);
            wait();
        }
    }*/
};

#endif // IMEM_H
