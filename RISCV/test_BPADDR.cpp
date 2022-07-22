#include<systemc.h>
#include "B_PADDR.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<sc_lv<32>>    addr_i{"addr_i"};
    sc_signal<sc_lv<2>>     ALIGNMENT {"ALIGNMENT"};
    sc_signal<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_signal<sc_lv<1>>     op2{"op2"};

    sc_signal<sc_lv<30>>    PADDR30{"PADDR"};

    blocPADDR BPADDR {"BPADDR"};

    BPADDR.clock(clock);
    BPADDR.addr_i(addr_i);
    BPADDR.ALIGNMENT(ALIGNMENT);
    BPADDR.first_cycle(first_cycle);
    BPADDR.op2(op2);
    BPADDR.PADDR30(PADDR30);

         sc_start();
         wf= sc_create_vcd_trace_file("test_BPADDR");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,addr_i,"addr_i");
         sc_trace(wf,ALIGNMENT,"ALIGNMENT");
         sc_trace(wf,first_cycle,"first_cycle");
         sc_trace(wf,op2,"op2");
         sc_trace(wf,PADDR30,"PADDR");

         addr_i.write(0);   first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         addr_i.write(0x1234567F); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         addr_i.write(0x1234567F); first_cycle.write(1);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         addr_i.write(0x1234567F); first_cycle.write(1);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         addr_i.write(0x1234567F); first_cycle.write(0);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         sc_close_vcd_trace_file(wf);
         return 0;
    }
