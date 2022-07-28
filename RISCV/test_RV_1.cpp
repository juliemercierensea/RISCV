/*!
 * \file test_RV_1.cpp
 * \brief testbench of the bloc RV_1
 */
#include<systemc.h>
#include "RV_1.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<bool>         RST{"RST"};
    sc_signal<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_signal<sc_lv<1>>     PREADY{"PREADY"};
    sc_signal<sc_lv<32>>    PADDR{"PADDR"};
    sc_signal<sc_lv<4>>     PSTRB{"PSTRB"};
    sc_signal<sc_lv<32>>    PWDATA{"PWDATA"};
    sc_signal<sc_lv<1>>     PWRITE{"PWRITE"};
    sc_signal<sc_lv<1>>     PENABLE{"PENABLE"};
    sc_signal<sc_lv<1>>     PREQ{"PREQ"};

    RV_1 RV1 {"RV_1"};

    RV1.clock(clock);
    RV1.RST(RST);
    RV1.PRDATA(PRDATA);
    RV1.PREADY(PREADY);
    RV1.PADDR(PADDR);
    RV1.PSTRB(PSTRB);
    RV1.PWDATA(PWDATA);
    RV1.PWRITE(PWRITE);
    RV1.PENABLE(PENABLE);
    RV1.PREQ(PREQ);

         sc_start();
         wf= sc_create_vcd_trace_file("test_RV_1");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,RST,"RST");
         sc_trace(wf,PRDATA,"PRDATA");
         sc_trace(wf,PREADY,"PREADY");
         sc_trace(wf,PADDR,"PADDR");
         sc_trace(wf,PSTRB,"PSTRB");
         sc_trace(wf,PWDATA,"PWDATA");
         sc_trace(wf,PWRITE,"PWRITE");
         sc_trace(wf,PENABLE,"PENABLE");
         sc_trace(wf,PREQ,"PREQ");

         RST.write(0);  PRDATA.write(0x0FF0000F); PREADY.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x0FF0000F); PREADY.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x0FF0000F); PREADY.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x0FF0000F); PREADY.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         /*RST.write(1);  PRDATA.write(0x00108093); PREADY.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x00108093); PREADY.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;*/

         sc_close_vcd_trace_file(wf);
         return 0;
    }
