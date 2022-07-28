/*!
 * \file IR.h
 * \brief register which writes the Loadval in Val on the rising edge following the fact that wIR=1 and membusy=0\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 11th 2022\n
 *
 */
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
    }

    void combinational () {
        Val.write(0);
        while (1){
            if (((wIR.read())&(~memBusy.read()))==1) {
                Val.write(LoadVal.read());
            }
        wait();
        }
    }
};

#endif // IR_H
