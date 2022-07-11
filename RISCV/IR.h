#include <systemc.h>
#ifndef IR_H
#define IR_H

SC_MODULE(IR) {
    sc_in_clk         clock;
    sc_out<sc_lv<32>> Val;
    sc_in<bool>       MemBusy;
    sc_in<bool>       wIR;

    sc_in<sc_lv<32>>  LoadVal;

    SC_CTOR(IR) {
        SC_THREAD(combinational);
        sensitive << clock.pos();
        sensitive << LoadVal;
        sensitive << wIR;
        sensitive<< MemBusy;
    }

    void combinational () {
        while (1){
        if (((wIR.read())&(~MemBusy.read()))==1) {
            Val.write(LoadVal.read());
        }
        wait();
        }
    }
};

#endif // IR_H
