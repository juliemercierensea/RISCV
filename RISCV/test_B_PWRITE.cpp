/*!
 * \file test_P_WRITE.cpp
 * \brief testbench of P_WRITE
 */
#include<systemc.h>
#include "B_PWRITE.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_signal<sc_lv<1>>     wr_i{"wr_i"};

    sc_signal<sc_lv<1>>    PWRITE{"PWRITE"};

    blocPWRITE BPWRITE {"BPWRITE"};

    BPWRITE.clock(clock);
    BPWRITE.first_cycle(first_cycle);
    BPWRITE.wr_i(wr_i);
    BPWRITE.PWRITE(PWRITE);

         sc_start();
         wf= sc_create_vcd_trace_file("test_BPWRITE");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,first_cycle,"first_cycle");
         sc_trace(wf,wr_i,"wr_i");
         sc_trace(wf,PWRITE,"PWRITE");

         wr_i.write(0);    first_cycle.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wr_i.write(0);    first_cycle.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wr_i.write(1);    first_cycle.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wr_i.write(1);    first_cycle.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;


         sc_close_vcd_trace_file(wf);
         return 0;
    }
