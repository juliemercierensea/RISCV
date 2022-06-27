#include <systemc.h>

#ifndef SIMPLESTATEMACHINE_H
#define SIMPLESTATEMACHINE_H

SC_MODULE (scStateMachine) {
    // ---------------------      Ports      ---------------------

    sc_in_clk           clock{"clock_i"};
    sc_in<bool>         RST{"reset_i"};
    sc_in<bool>         trigger{"trigger_i"};
    sc_in<bool>         PREADY{"PREADY_i"};
    sc_in<bool>         unaligned{"unaligned_i"};
    sc_in<bool>         condition;

    sc_out<bool>    op1;
    sc_out<bool>    op2;
    sc_out<bool>    first_cycle;
    sc_out<sc_lv<2>>    busy_sel;
    sc_out<bool>    preq_sel;
    sc_out<bool>    PENABLE;
    sc_out<sc_lv<2>>    ShowCurrentState;

    sc_lv <2>       CurrentState;

    SC_CTOR(scStateMachine) {
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
                CurrentState=0;
                ShowCurrentState.write(CurrentState);
            }
            else if(CurrentState==0 && trigger==1){ //stOp1A -> stOp1B
                while(trigger==1){
                    CurrentState=1;
                    ShowCurrentState.write(CurrentState);
                    wait();
                }
            }
            else if(CurrentState==1 && PREADY==1 && unaligned==0){ //stOp1B -> stOp1A
                while(PREADY==1 && unaligned==0){
                    CurrentState=0;
                    ShowCurrentState.write(CurrentState);
                    wait();
                }
            }
            else if(CurrentState==1 && PREADY==1 && unaligned==1){ //stOp1B-> stOp2A
                while(PREADY==1 && unaligned==1){
                    CurrentState=0x10;
                    ShowCurrentState.write(CurrentState);
                    wait();
                }
            }
            else if(CurrentState==0x10 && condition==1){ //stOp2A-> stOp2B
                while(condition==1){
                    CurrentState=0x11;
                    ShowCurrentState.write(CurrentState);
                    wait();
                }
            }
            else if(CurrentState==0x11 && PREADY==1){ //stOp2B-> stOp1A
                while(PREADY==1){
                    CurrentState=0;
                    ShowCurrentState.write(CurrentState);
                    wait();
                }
            }

            if(CurrentState==0){        //stOp1A
                op1.write(1);
                op2.write(0);
                first_cycle.write(1);
                busy_sel.write(0);
                preq_sel.write(0);
                PENABLE.write(0);
            }
            if(CurrentState==1){        //stOp1B
                op1.write(1);
                op2.write(0);
                first_cycle.write(0);
                busy_sel.write(1);
                preq_sel.write(1);
                PENABLE.write(1);
            }
            if(CurrentState==0x10){        //stOp2A
                op1.write(0);
                op2.write(1);
                first_cycle.write(0);
                busy_sel.write(0x10);
                preq_sel.write(1);
                PENABLE.write(0);
            }
            if(CurrentState==0x11){        //stOp2B
                op1.write(0);
                op2.write(1);
                first_cycle.write(0);
                busy_sel.write(0x11);
                preq_sel.write(1);
                PENABLE.write(1);
            }

        wait();
        }
    }
};

#endif // SIMPLESTATEMACHINE_H
