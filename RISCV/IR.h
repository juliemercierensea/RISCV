#ifndef IR_H
#define IR_H
#include <systemc.h>

SC_MODULE(IR) {
    sc_in_clk         clock{"clock"};
    sc_out<sc_lv<32>> Val{"Val"};
    sc_in<sc_lv<1>>   memBusy{"memBusy"};
    sc_in<sc_lv<1>>   wIR{"wIR"};

    sc_in<sc_lv<32>>  LoadVal{"LoadVal"};

    SC_CTOR(IR) {
        SC_THREAD(combinational);
        sensitive << clock.pos();
        sensitive << LoadVal;
        sensitive << wIR;
        sensitive<<  memBusy;
    }

    void combinational () {
        while (1){
        if (((wIR.read())&(~memBusy.read()))==1) {
            Val.write(LoadVal.read());
        }
        wait();
        }
    }
};

#endif // IR_H
