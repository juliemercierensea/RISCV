#include<systemc.h>
#include "AMBA.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool> RST{"RST"};
    sc_signal<bool>   clock;

    AMBA AMBA {"AMBA"};

    AMBA.clock(clock);
    AMBA.RST(RST);

         sc_start();
         wf= sc_create_vcd_trace_file("test_AMBA");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,RST,"reset");

         RST.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1; sc_start(1,SC_NS);

         for (int i=0; i<30; i++){

         RST.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;
         }

         RST.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         sc_close_vcd_trace_file(wf);
         return 0;
    }
