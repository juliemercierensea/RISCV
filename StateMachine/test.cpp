#include <systemc.h>
#include"State.h"
#include "StateOutput.h"

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>   clock;
    sc_signal<bool> RST("reset");
    sc_signal<bool> trigger;
    sc_signal<bool> PREADY;
    sc_signal<bool> unaligned;
    sc_signal<bool> condition;

    sc_signal<bool> op1;
    sc_signal<bool> op2;
    sc_signal<bool> first_cycle;
    sc_signal<sc_lv<2>> busy_sel;
    sc_signal<bool> preq_sel;
    sc_signal<bool> PENABLE;
    sc_signal<bool> CurrentState;
    sc_signal<sc_lv<2>> CS_StatetoStateOutput;
    sc_signal<sc_lv<2>> ShowCurrentState;

    scState State("AMBA_State");
    scStateOutput StateOutput("AMBA_StateOutput");

    State.clock(clock);
    State.RST(RST);
    State.trigger(trigger);
    State.PREADY(PREADY);
    State.unaligned(unaligned);
    State.condition(condition);
    State.CurrentState(CS_StatetoStateOutput);

    StateOutput.clock(clock);
    StateOutput.op1(op1);
    StateOutput.op2(op2);
    StateOutput.busy_sel(busy_sel);
    StateOutput.first_cycle(first_cycle);
    StateOutput.preq_sel(preq_sel);
    StateOutput.PENABLE(PENABLE);
    StateOutput.CurrentState_i(CS_StatetoStateOutput);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("StateMachine_RST=1.v2");

     sc_trace(wf,clock,"clock");
     sc_trace(wf,RST,"reset");
     sc_trace(wf,trigger,"trigger");
     sc_trace(wf,PREADY,"PREADY");
     sc_trace(wf,unaligned,"unaligned");
     sc_trace(wf,op1,"op1");
     sc_trace(wf,op2,"op2");
     sc_trace(wf,first_cycle,"first_cycle");
     sc_trace(wf,busy_sel,"busy_sel");
     sc_trace(wf,preq_sel,"preq_sel");
     sc_trace(wf,PENABLE,"PENABLE");
     sc_trace(wf,CS_StatetoStateOutput,"CS_StatetoStateOutput");
     sc_trace(wf,condition,"condition");


     for (int i=0;i<2;i++) { //stop1A OK
         RST.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
     }
     for (int i=0;i<2;i++) { //stop1A -> stop1B OK
         RST.write(0);
         trigger.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
     }

     /*for (int i=0;i<2;i++) {    //stop1B -> stop1A OK

         RST.write(0);
         trigger.write(0);
         PREADY.write(1);
         unaligned.write(0);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
         RST.write(1);
     }*/

     for (int i=0;i<2;i++) {    //stop1B -> stop2A
         RST.write(0);
         trigger.write(0);
         PREADY.write(1);
         unaligned.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {    //stop2A -> stop2B
         RST.write(0);
         PREADY.write(0);
         condition.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
     }

     for (int i=0;i<2;i++) {    //stop2B -> stop1A

         PREADY.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
     }

     sc_close_vcd_trace_file(wf);
     return 0;
}
