#include<systemc.h>
#include "Memory_Interface.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<bool>         RST{"RST"};
    sc_signal<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_signal<sc_lv<1>>     PREADY{"PREADY"};
    sc_signal<sc_lv<1>>     rd_i{"rd_i"};
    sc_signal<sc_lv<1>>     wr_i{"wr_i"};
    sc_signal<sc_lv<32>>    addr_i{"addr_i"};
    sc_signal<sc_lv<2>>     size_i{"size_i"};
    sc_signal<sc_lv<1>>     unsigned_i{"unsigned_i"};
    sc_signal<sc_lv<32>>    wdata_i{"wdata_i"};

    sc_signal<sc_lv<32>>    PADDR{"PADDR"};
    sc_signal<sc_lv<4>>     PSTRB{"PSTRB"};
    sc_signal<sc_lv<32>>    PWDATA{"PWDATA"};
    sc_signal<sc_lv<1>>     PWRITE{"PWRITE"};
    sc_signal<sc_lv<1>>     PENABLE{"PENABLE_super"};
    sc_signal<sc_lv<1>>     PREQ{"PREQ"};
    sc_signal<sc_lv<1>>     busy_o{"busy_o"};
    sc_signal<sc_lv<32>>    rdata_o{"rdata_o"};
    sc_signal<sc_lv<32>>    rdata_unbuff_o{"rdata_unbuff_o"};

    scMemory_Interface MemInt {"MemInt"};

    MemInt.clock(clock);
    MemInt.RST(RST);
    MemInt.PRDATA(PRDATA);
    MemInt.PREADY(PREADY);
    MemInt.rd_i(rd_i);
    MemInt.wr_i(wr_i);
    MemInt.addr_i(addr_i);
    MemInt.size_i(size_i);
    MemInt.unsigned_i(unsigned_i);
    MemInt.wdata_i(wdata_i);

    MemInt.PADDR(PADDR);
    MemInt.PSTRB(PSTRB);
    MemInt.PWDATA(PWDATA);
    MemInt.PWRITE(PWRITE);
    MemInt.PENABLE(PENABLE);
    MemInt.PREQ(PREQ);
    MemInt.busy_o(busy_o);
    MemInt.rdata_o(rdata_o);
    MemInt.rdata_unbuff_o(rdata_unbuff_o);

         sc_start();
         wf= sc_create_vcd_trace_file("test_MemInt");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,RST,"RST");
         sc_trace(wf,PRDATA,"PRDATA");
         sc_trace(wf,PREADY,"PREADY");
         sc_trace(wf,rd_i,"rd_i");
         sc_trace(wf,wr_i,"wr_i");
         sc_trace(wf,addr_i,"addri_");
         sc_trace(wf,size_i,"size_i");
         sc_trace(wf,unsigned_i,"unsigned_i");
         sc_trace(wf,wdata_i,"wdata_i");

         sc_trace(wf,PADDR,"PADDR");
         sc_trace(wf,PSTRB,"PSTRB");
         sc_trace(wf,PWDATA,"PWDATA");
         sc_trace(wf,PWRITE,"PWRITE");
         sc_trace(wf,PENABLE,"PENABLE");
         sc_trace(wf,PREQ,"PREQ");
         sc_trace(wf,busy_o,"busy_o");
         sc_trace(wf,rdata_o,"rdata_o");
         sc_trace(wf,rdata_unbuff_o,"rdata_unbff_o");

         RST.write(0);  PRDATA.write(0); PREADY.write(0);   rd_i.write(0);
         wr_i.write(0);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x93); PREADY.write(0);   rd_i.write(0);
         wr_i.write(0);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x93); PREADY.write(1);   rd_i.write(0);
         wr_i.write(0);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x93); PREADY.write(1);   rd_i.write(0);
         wr_i.write(0);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x93); PREADY.write(1);   rd_i.write(0);
         wr_i.write(1);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  PRDATA.write(0x93); PREADY.write(1);   rd_i.write(1);
         wr_i.write(1);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(0);  PRDATA.write(0x93); PREADY.write(1);   rd_i.write(1);
         wr_i.write(1);  addr_i.write(0); size_i.write(0);   unsigned_i.write(0); wdata_i.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;


         sc_close_vcd_trace_file(wf);
         return 0;
    }
