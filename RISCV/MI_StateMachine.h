
#ifndef MI_STATEMACHINE_H
#define MI_STATEMACHINE_H
#include <systemc.h>

SC_MODULE (scState) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock"};
    sc_in<bool>         RST{"reset"};
    sc_in<sc_lv<1>>     trigger{"trigger"};
    sc_in<sc_lv<1>>     PREADY{"PREADY"};
    sc_out<sc_lv<1>>    notPREADY{"notPREADY"};
    sc_out<sc_lv<1>>    unaligned_or_notPREADY{"unaligned_or_notPREADY"};

    sc_in<sc_lv<1>>     unaligned{"unaligned"};
    sc_in<bool>         condition{"condition"};

    sc_out<sc_lv<2>>    CurrentState{"CurrentState"};

    SC_CTOR(scState) {
        SC_THREAD(state);
        sensitive << clock.pos();
        sensitive << RST;
        }

    void state() {
        while (1){

            notPREADY.write(~PREADY.read());
            unaligned_or_notPREADY.write(unaligned.read()|notPREADY.read());

            if(RST==0){     //stOp1A
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

