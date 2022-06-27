#include <systemc.h>
#ifndef STATE_H
#define STATE_H

SC_MODULE (scState) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<bool>         RST{"reset_i"};
    sc_in<bool>         trigger{"trigger_i"};
    sc_in<bool>         PREADY{"PREADY_i"};
    sc_in<bool>         unaligned{"unaligned_i"};
    sc_in<bool>         condition;

    sc_out<sc_lv<2>>    CurrentState;

    SC_CTOR(scState) {
        SC_THREAD(state);
        sensitive << clock.pos();
        sensitive << RST;
        //sensitive << PREADY;
        //sensitive << trigger;
        //sensitive << unaligned;

        }

    void state() {
        while (1){
            if(RST==1){     //stOp1A
                CurrentState.write(0);
            }
            else{
                if((CurrentState.read().to_uint()==0) && (trigger==1)){ //stOp1A -> stOp1B
                    CurrentState.write(1);
                }
                else{
                    if((CurrentState.read()==1) && (PREADY==1) && (unaligned==0)){ //stOp1B -> stOp1A
                        CurrentState.write(0);
                    }
                    else{
                        if((CurrentState.read()==1) && (PREADY==1) && (unaligned==1)){ //stOp1B-> stOp2A
                            CurrentState.write(2);
                        }
                        else{
                            if((CurrentState.read()==2) && (condition==1)){ //stOp2A-> stOp2B
                                CurrentState.write(3);
                            }
                            else {
                                 if((CurrentState.read()==0x3) && (PREADY==1)){ //stOp2B-> stOp1A
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

#endif // STATE_H
