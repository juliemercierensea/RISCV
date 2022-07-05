#include <systemc.h>
#ifndef STATEOUTPUT_H
#define STATEOUTPUT_H

SC_MODULE (scStateOutput) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<sc_lv<2>>     CurrentState_i;

    sc_out<bool>        op1;
    sc_out<sc_lv<1>>        op2;
    sc_out<sc_lv<1>>        first_cycle;
    sc_out<sc_lv<2>>    busy_sel;
    sc_out<bool>        preq_sel;
    sc_out<bool>        PENABLE;

    SC_CTOR(scStateOutput) {
        SC_THREAD(state);
        sensitive << clock.pos();
        sensitive << CurrentState_i;
        }

    void state() {
        while (1){

            if(CurrentState_i.read().to_uint()==0){        //stOp1A
                op1.write(1);
                op2.write(0);
                first_cycle.write(1);
                busy_sel.write(0);
                preq_sel.write(0);
                PENABLE.write(0);
            }
            else if(CurrentState_i.read().to_uint()==1){        //stOp1B
                op1.write(1);
                op2.write(0);
                first_cycle.write(0);
                busy_sel.write(1);
                preq_sel.write(1);
                PENABLE.write(1);
            }
            else if(CurrentState_i.read().to_uint()==2){        //stOp2A
                op1.write(0);
                op2.write(1);
                first_cycle.write(0);
                busy_sel.write(2);
                preq_sel.write(1);
                PENABLE.write(0);
            }
            else if(CurrentState_i.read().to_uint()==3){        //stOp2B
                op1.write(0);
                op2.write(1);
                first_cycle.write(0);
                busy_sel.write(3);
                preq_sel.write(1);
                PENABLE.write(1);
            }

        wait();
        }
    }
};

#endif // STATEOUTPUT_H

