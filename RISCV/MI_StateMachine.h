#include <systemc.h>
#ifndef MI_STATEMACHINE_H
#define MI_STATEMACHINE_H

SC_MODULE (scState) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<bool>         RST{"reset_i"};
    sc_in<sc_lv<1>>     trigger{"trigger_i"};
    sc_in<sc_lv<1>>     PREADY{"PREADY_i"};
    sc_out<sc_lv<1>>    notPREADY;
    sc_out<sc_lv<1>>    unaligned_or_notPREADY;

    sc_in<sc_lv<1>>     unaligned{"unaligned_i"};
    sc_in<bool>         condition;

    sc_out<sc_lv<2>>    CurrentState;

    SC_CTOR(scState) {
        SC_THREAD(state);
        sensitive << clock.pos();
        sensitive << RST;
        }

    void state() {
        while (1){

            notPREADY.write(~PREADY.read());
            unaligned_or_notPREADY.write(unaligned.read()|notPREADY.read());

            if(RST==1){     //stOp1A
                CurrentState.write(0);
            }
            else{
                if((CurrentState.read().to_uint()==0) && (trigger.read()==1)){ //stOp1A -> stOp1B
                    CurrentState.write(1);
                }
                else{
                    if((CurrentState.read()==1) && (PREADY.read()==1) && (unaligned.read()==0)){ //stOp1B -> stOp1A
                        CurrentState.write(0);
                    }
                    else{
                        if((CurrentState.read()==1) && (PREADY.read()==1) && (unaligned.read()==1)){ //stOp1B-> stOp2A
                            CurrentState.write(2);
                        }
                        else{
                            if((CurrentState.read()==2) && (condition==1)){ //stOp2A-> stOp2B
                                CurrentState.write(3);
                            }
                            else {
                                 if((CurrentState.read()==0x3) && (PREADY.read()==1)){ //stOp2B-> stOp1A
                                    CurrentState.write(0);
                                 }
                            }
                        }
                    }
                }
            }

        wait();
        }
    }
};

#endif // MI_STATEMACHINE_H

