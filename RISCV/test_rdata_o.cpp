/*!
 * \file test_rdata_o.cpp
 * \brief testbench of the bloc rdata_o
 */
#include<systemc.h>
#include "B_rdata_o.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_signal<sc_lv<1>>     op1{"op1"};
    sc_signal<sc_lv<1>>     PREADY{"PREADY"};
    sc_signal<sc_lv<1>>     op2{"op2"};
    sc_signal<sc_lv<2>>     ALIGNMENT_REG{"ALIGNMENT_REG"};
    sc_signal<sc_lv<1>>     unsigned_i{"unsigned_i"};
    sc_signal<sc_lv<2>>     size_i{"size_i"};

    sc_signal<sc_lv<32>>   rdata_o{"rdata_o"};
    sc_signal<sc_lv<32>>   rdata_unbuff_o{"rdata_unbuff_o"};

    blocRdata_o BRDATAO {"BRDATAO"};

    BRDATAO.clock(clock);
    BRDATAO.PRDATA(PRDATA);
    BRDATAO.op1(op1);
    BRDATAO.PREADY(PREADY);
    BRDATAO.op2(op2);
    BRDATAO.ALIGNMENT_REG(ALIGNMENT_REG);
    BRDATAO.unsigned_i(unsigned_i);
    BRDATAO.size_i(size_i);
    BRDATAO.rdata_o(rdata_o);
    BRDATAO.rdata_unbuff_o(rdata_unbuff_o);

         sc_start();
         wf= sc_create_vcd_trace_file("test_BRDATA_O");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,PRDATA,"PRDATA");
         sc_trace(wf,op1,"op1");
         sc_trace(wf,PREADY,"PREADY");
         sc_trace(wf,op2,"op2");
         sc_trace(wf,ALIGNMENT_REG,"ALIGNMENT_REG");
         sc_trace(wf,unsigned_i,"unsigned_i");
         sc_trace(wf,size_i,"size_i");
         sc_trace(wf,rdata_o,"rdata_o");
         sc_trace(wf,rdata_unbuff_o,"rdata_unbuff_o");

         PRDATA.write(0x93); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x93); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(1);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x8000); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(1);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x8000); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(1);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(2);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(0);    op2.write(1);   ALIGNMENT_REG.write(3);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(1);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(1);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(1);  PREADY.write(1);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x12345678); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(0);
         unsigned_i.write(0);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(0);    op2.write(1);   ALIGNMENT_REG.write(0);
         unsigned_i.write(1);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(0);    op2.write(0);   ALIGNMENT_REG.write(1);
         unsigned_i.write(1);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         PRDATA.write(0x1234567F); op1.write(0);  PREADY.write(1);    op2.write(0);   ALIGNMENT_REG.write(1);
         unsigned_i.write(1);   size_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         sc_close_vcd_trace_file(wf);
         return 0;
    }
