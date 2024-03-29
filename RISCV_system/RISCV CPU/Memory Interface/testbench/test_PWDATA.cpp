/*!
 * \file test_PWDATA.cpp
 * \brief testbench of the bloc PWDATA
 */
#include<systemc.h>
#include "B_PWDATA.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<sc_lv<32>>    wdata_i{"wdata_i"};
    sc_signal<sc_lv<2>>     ALIGNMENT {"ALIGNMENT"};
    sc_signal<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_signal<sc_lv<1>>     op2{"op2"};

    sc_signal<sc_lv<32>>    PWDATA{"PWDATA"};

    blocPWDATA BPWDATA {"BPWDATA"};

    BPWDATA.clock(clock);
    BPWDATA.wdata_i(wdata_i);
    BPWDATA.ALIGNMENT(ALIGNMENT);
    BPWDATA.first_cycle(first_cycle);
    BPWDATA.op2(op2);
    BPWDATA.PWDATA(PWDATA);

         sc_start();
         wf= sc_create_vcd_trace_file("test_BPWDATA");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,wdata_i,"wdata_i");
         sc_trace(wf,ALIGNMENT,"ALIGNMENT");
         sc_trace(wf,first_cycle,"first_cycle");
         sc_trace(wf,op2,"op2");
         sc_trace(wf,PWDATA,"PWDATA");

         wdata_i.write(0);   ALIGNMENT.write(0); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x1000);   ALIGNMENT.write(0); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x12345678);   ALIGNMENT.write(0); first_cycle.write(0);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x1000);   ALIGNMENT.write(0); first_cycle.write(1);   op2.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x1000);   ALIGNMENT.write(0); first_cycle.write(1);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x1000);   ALIGNMENT.write(1); first_cycle.write(1);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x1000);   ALIGNMENT.write(1); first_cycle.write(0);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         wdata_i.write(0x1000);   ALIGNMENT.write(1); first_cycle.write(1);   op2.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         sc_close_vcd_trace_file(wf);
         return 0;
    }
