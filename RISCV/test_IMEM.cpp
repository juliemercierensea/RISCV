#include<systemc.h>
#include "AMBA.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>          PCLK{"clock_i"};
    sc_signal<sc_lv<8>>      PADDR{"PADDR"};
    sc_signal<sc_lv<4>>      PSTRB{"PSTRB"};
    sc_signal<sc_lv<1>>      PENABLE{"PENABLE_super"};
    sc_signal<sc_lv<2>>      PSEL{"PSEL"};

    sc_signal<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_signal<sc_lv<1>>     PREADY{"PREADY"};

    scIMEM IMEM {"test1.hex","IMEM"};

    IMEM.PCLK(PCLK);
    IMEM.PADDR(PADDR);
    IMEM.PSTRB(PSTRB);
    IMEM.PENABLE(PENABLE);
    IMEM.PSEL(PSEL);
    IMEM.PRDATA(PRDATA);
    IMEM.PREADY(PREADY);

         sc_start();
         wf= sc_create_vcd_trace_file("test_IMEM");

         sc_trace(wf,PCLK,"clock");
         sc_trace(wf,PADDR,"PADDR");
         sc_trace(wf,PSTRB,"PSTRB");
         sc_trace(wf,PENABLE,"PENABLE");
         sc_trace(wf,PSEL,"PSEL");
         sc_trace(wf,PRDATA,"PRDATA");
         sc_trace(wf,PREADY,"PREADY");

         PSTRB.write(0);    PSEL.write(0);  PENABLE.write(0);   PADDR.write(1);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0);    PSEL.write(0);  PENABLE.write(1);   PADDR.write(1);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(1);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(0);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(1);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(2);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b11);  PENABLE.write(1);   PADDR.write(3);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(4);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(5);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(6);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;

         PSTRB.write(0b1101);    PSEL.write(0b1);  PENABLE.write(1);   PADDR.write(7);
         sc_start(10, SC_NS); PCLK=0; sc_start(10, SC_NS); PCLK =1;


         sc_close_vcd_trace_file(wf);
         return 0;
    }
