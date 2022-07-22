#include<systemc.h>
#include "MI_registre.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>         clock{"clock"};
    sc_signal<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_signal<sc_lv<2>>     ALIGNMENT{"ALIGNMENT"};
    sc_signal<sc_lv<2>>     ALIGNMENT_REG{"ALIGNMENT_REG"};

    registre<2> ALIGNMENTreg {"ALIGNMENTreg"};

    ALIGNMENTreg.clock(clock);
    ALIGNMENTreg.load(first_cycle);
    ALIGNMENTreg.d(ALIGNMENT);
    ALIGNMENTreg.q(ALIGNMENT_REG);

         sc_start();
         wf= sc_create_vcd_trace_file("test_ALIGNMENTregister");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,first_cycle,"first_cycle");
         sc_trace(wf,ALIGNMENT,"ALIGNMENT");
         sc_trace(wf,ALIGNMENT_REG,"ALIGNMENT_REG");

         ALIGNMENT.write(0);    first_cycle.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         ALIGNMENT.write(1);    first_cycle.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         ALIGNMENT.write(1);    first_cycle.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         ALIGNMENT.write(0b11);    first_cycle.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         sc_close_vcd_trace_file(wf);
         return 0;
    }
