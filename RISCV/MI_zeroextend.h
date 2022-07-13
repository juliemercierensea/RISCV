#include <systemc.h>
#ifndef MI_ZEROEXTEND_H
#define MI_ZEROEXTEND_H

SC_MODULE (scZeroextend) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    in{"in"};
    sc_out<sc_lv<64>>   out{"out"};

    SC_CTOR(scZeroextend) {
        SC_THREAD(extend);
        sensitive << in;
        }

    void extend() {
        while (1){
            out.write(in.read());
        wait();
        }
    }
};


#endif // MI_ZEROEXTEND_H
