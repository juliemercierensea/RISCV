#ifndef B_STATEMACHINE_H
#define B_STATEMACHINE_H
#include <systemc.h>
#include "MI_StateMachine.h"
#include "MI_StateOutput.h"
#include "trace.h"

SC_MODULE(blocSTATEMACHINE){

    sc_in<bool>     RST{"reset"};
    sc_in<sc_lv<1>> trigger{"trigger"};
    sc_in<sc_lv<1>> PREADY{"PREADY"};
    sc_in<sc_lv<1>> unaligned{"unaligned"};
    sc_in_clk       clock{"clock"};

    sc_out<sc_lv<1>>    op1{"op1"};
    sc_out<sc_lv<1>>    op2{"op2"};
    sc_out<sc_lv<1>>    first_cycle{"first_cycle"};
    sc_out<sc_lv<2>>    busy_sel{"busy_sel"};
    sc_out<sc_lv<1>>    preq_sel{"preq_sel"};
    sc_out<sc_lv<1>>    PENABLE{"PENABLE"};

    scState         State {"MemInt_State"};
    scStateOutput   StateOutput {"MemInt_StateOutput"};

    sc_signal<bool>         condition{"condition"};
    sc_signal<sc_lv<1>>     unaligned_or_notPREADY{"unaligned_or_notPREADY"};
    sc_signal<sc_lv<1>>     notPREADY{"notPREADY"};
    sc_signal<sc_lv<2>>     CS_StatetoStateOutput{"CS_StatetoStateOutput"};


    SC_CTOR(blocSTATEMACHINE){

        State.clock(clock);
        State.RST(RST);
        State.trigger(trigger);
        State.PREADY(PREADY);
        State.unaligned(unaligned);
        State.condition(condition);
        State.CurrentState(CS_StatetoStateOutput);
        State.notPREADY(notPREADY);
        State.unaligned_or_notPREADY(unaligned_or_notPREADY);

        StateOutput.clock(clock);
        StateOutput.op1(op1);
        StateOutput.op2(op2);
        StateOutput.busy_sel(busy_sel);
        StateOutput.first_cycle(first_cycle);
        StateOutput.preq_sel(preq_sel);
        StateOutput.PENABLE(PENABLE);
        StateOutput.CurrentState_i(CS_StatetoStateOutput);
    }
};


#endif // B_STATEMACHINE_H
