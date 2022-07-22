
#ifndef MI_PLUS_PADDR_H
#define MI_PLUS_PADDR_H
#include<systemc.h>

SC_MODULE (scPlus) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<30>>    in_up{"in_up"};
    sc_out<sc_lv<30>>   out{"out"};

    SC_CTOR(scPlus) {
        SC_THREAD(operation);
        sensitive <<in_up;
        }

    void operation() {
        while (1){
            out.write(in_up.read().to_uint()+0x1);

        wait();
        }
    }
};


#endif // MI_PLUS_PADDR_H
