#include<systemc.h>
#ifndef MI_PLUS_PADDR_H
#define MI_PLUS_PADDR_H

SC_MODULE (scPlus) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<30>>    in_up;
    sc_out<sc_lv<30>>   out;
    sc_out<sc_lv<1>>   one;

    SC_CTOR(scPlus) {
        SC_THREAD(operation);
        }

    void operation() {
        while (1){
            out.write(in_up.read().to_uint()+0x1);
            one.write(1);
        wait();
        }
    }
};


#endif // MI_PLUS_PADDR_H
