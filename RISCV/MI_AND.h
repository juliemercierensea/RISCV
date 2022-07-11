#include<systemc.h>
#ifndef MI_AND_H
#define MI_AND_H

SC_MODULE (scAND) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<1>>        op1;
    sc_in<sc_lv<1>>        PREADY;

    sc_out<sc_lv<1>>         out;

    SC_CTOR(scAND) {
        SC_THREAD(logicAND);
        sensitive<<op1;
        sensitive<<PREADY;
        }

    void logicAND() {
        while (1){
            out.write(op1.read()&PREADY.read());

        wait();
        }
    }
};

#endif // MI_AND_H
