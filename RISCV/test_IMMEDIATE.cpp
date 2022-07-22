#include<systemc.h>
#include "IMMEDIATE.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<sc_lv<32>>     I{"I"};
    sc_signal<sc_lv<32>>     S{"S"};
    sc_signal<sc_lv<32>>     U{"U"};
    sc_signal<sc_lv<32>>     B{"B"};
    sc_signal<sc_lv<32>>     J{"J"};

    sc_signal<sc_lv<32>>   RI{"RI"};

    scIMMEDIATE IMM {"IMMEDIATE"};

    IMM.I(I);
    IMM.S(S);
    IMM.U(U);
    IMM.B(B);
    IMM.J(J);
    IMM.RI(RI);

         sc_start();
         wf= sc_create_vcd_trace_file("test_IMMEDIATE");

         sc_trace(wf,I,"I");
         sc_trace(wf,S,"S");
         sc_trace(wf,U,"U");
         sc_trace(wf,B,"B");
         sc_trace(wf,J,"J");
         sc_trace(wf,RI,"RI");

         RI.write(0x0);   sc_start(10, SC_NS);
         RI.write(0b1011001101010);   sc_start(10, SC_NS);
         RI.write(0b0011111100001);   sc_start(10, SC_NS);
         //RI.write(0b111111111111);   sc_start(10, SC_NS);

         sc_close_vcd_trace_file(wf);
         return 0;
    }
