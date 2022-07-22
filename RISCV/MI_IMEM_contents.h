
#ifndef MI_IMEM_CONTENTS_H
#define MI_IMEM_CONTENTS_H
#include "IHex.h"
#include "IHex.cpp"
#include <systemc.h>
SC_MODULE (scIMEM) {
    sc_lv<32> contents [256];

    typedef scIMEM SC_CURRENT_USER_MODULE;                          \
    scIMEM(const char *fname, ::sc_core::sc_module_name = "")
    {
        IHex::IHexFile ihexfile(fname);
        //ihexfile.hasError();
        ihexfile.exportSystemC(0,256,contents);
    }

    /*void state() {
        while (1){
            ihexfile.exportSystemC(0, 256, contents);
            wait();
        }
    }*/
};

#endif // MI_IMEM_CONTENTS_H
