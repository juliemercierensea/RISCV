#include<systemc.h>
#include "AMBA.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool> RST{"RST"};
    sc_signal<bool>   clock;
    sc_signal <sc_lv<3>> func3;

    AMBA AMBA {"AMBA"};

    AMBA.clock(clock);
    AMBA.RST(RST);

         sc_start();
         wf= sc_create_vcd_trace_file("generaltest1");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,RST,"reset");
         sc_trace(wf,func3,"func3");
         /*sc_trace(wf,INUM,"INUM");
         sc_trace(wf,opcode_6to2,"opcode_6to2");
         sc_trace(wf,uINSTR,"uINSTR");
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
         sc_trace(wf,condition,"condition");*/

         RST.write(0);
         //condition.write(1);

         for (int i=0;i<8;i++) {
             clock=0;
             sc_start(10, SC_NS);
             clock =1;
             sc_start(10, SC_NS);
         }

         sc_close_vcd_trace_file(wf);
         return 0;
    }
