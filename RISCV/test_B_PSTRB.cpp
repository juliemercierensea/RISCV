#include<systemc.h>
#include "B_PSTRB.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<sc_lv<2>>     size_i{"size_i"};
    sc_signal<sc_lv<2>>     ALIGNMENT {"ALIGNMENT"};
    sc_signal<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_signal<sc_lv<1>>     op2{"op2"};

    sc_signal<sc_lv<4>>    PSTRB{"PSTRB"};
    sc_signal<sc_lv<1>>    unaligned{"unaligned"};

    blocPSTRB BPSTRB {"BPSTRB"};

    BPSTRB.clock(clock);
    BPSTRB.size_i(size_i);
    BPSTRB.ALIGNMENT(ALIGNMENT);
    BPSTRB.first_cycle(first_cycle);
    BPSTRB.op2(op2);
    BPSTRB.PSTRB(PSTRB);
    BPSTRB.unaligned(unaligned);

         sc_start();
         wf= sc_create_vcd_trace_file("test_BPSTRB");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,size_i,"size_i");
         sc_trace(wf,ALIGNMENT,"ALIGNMENT");
         sc_trace(wf,first_cycle,"first_cycle");
         sc_trace(wf,op2,"op2");
         sc_trace(wf,PSTRB,"PSTRB");
         sc_trace(wf,unaligned,"unaligned");



         size_i.write(0);   ALIGNMENT.write(3); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         size_i.write(0);    ALIGNMENT.write(3); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         size_i.write(0);    ALIGNMENT.write(3); first_cycle.write(1);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         size_i.write(0b1);   ALIGNMENT.write(3); first_cycle.write(1);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         size_i.write(0b11);  ALIGNMENT.write(3); first_cycle.write(1);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         size_i.write(0b10);  ALIGNMENT.write(3); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         size_i.write(0b10);  ALIGNMENT.write(3); first_cycle.write(0);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;



         sc_close_vcd_trace_file(wf);
         return 0;
    }
