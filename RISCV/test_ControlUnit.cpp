#include<systemc.h>
#include "Control_Unit.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<sc_lv<32>>    instruction{"instruction"};
    sc_signal<sc_lv<1>>     memBusy{"memBusy"};
    sc_signal<bool>         clock{"clock"};
    sc_signal<bool>         RST{"reset"};

    sc_signal<sc_lv<1>>    sel1PC{"sel1PC"};
    sc_signal<sc_lv<1>>    sel2PC{"sel2PC"};
    sc_signal<sc_lv<1>>    iPC{"iPC"};
    sc_signal<sc_lv<1>>    wRD{"wRD"};
    sc_signal<sc_lv<1>>    selRD{"selRD"};
    sc_signal<sc_lv<1>>    sel1ALU{"sel1ALU"};
    sc_signal<sc_lv<2>>    sel2ALU{"sel2ALU"};
    sc_signal<sc_lv<4>>    selopALU{"selopALU"};
    sc_signal<sc_lv<1>>    wIR{"wIR"};
    sc_signal<sc_lv<1>>    RD{"RD"};
    sc_signal<sc_lv<1>>    WR{"WR"};
    sc_signal<sc_lv<1>>    IDMEM{"IDMEM"};

    scControl_Unit CU {"Control_Unit"};

    CU.clock(clock);
    CU.RST(RST);
    CU.instruction(instruction);
    CU.memBusy(memBusy);
    CU.sel1PC(sel1PC);
    CU.sel2PC(sel2PC);
    CU.iPC(iPC);
    CU.wRD(wRD);
    CU.selRD(selRD);
    CU.sel1ALU(sel1ALU);
    CU.sel2ALU(sel2ALU);
    CU.selopALU(selopALU);
    CU.wIR(wIR);
    CU.RD(RD);
    CU.WR(WR);
    CU.IDMEM(IDMEM);

         sc_start();
         wf= sc_create_vcd_trace_file("test_ControlUnit");

         sc_trace(wf,clock,"clock");
         sc_trace(wf,RST,"RST");
         sc_trace(wf,instruction,"instruction");
         sc_trace(wf,memBusy,"memBusy");
         sc_trace(wf,sel1PC,"sel1PC");
         sc_trace(wf,sel2PC,"sel2PC");
         sc_trace(wf,iPC,"iPC");
         sc_trace(wf,wRD,"wRD");
         sc_trace(wf,selRD,"selRD");
         sc_trace(wf,sel1ALU,"sel1ALU");
         sc_trace(wf,sel2ALU,"sel2ALU");
         sc_trace(wf,selopALU,"selopALU");
         sc_trace(wf,wIR,"wIR");
         sc_trace(wf,RD,"RD");
         sc_trace(wf,WR,"WR");
         sc_trace(wf,IDMEM,"IDMEM");

         RST.write(0);  instruction.write(0x0FF0000F);  memBusy.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(0);  instruction.write(0x0FF0000F);  memBusy.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  instruction.write(0x0FF0000F);  memBusy.write(1);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         RST.write(1);  instruction.write(0x0FF0000F);  memBusy.write(0);
         sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

         sc_close_vcd_trace_file(wf);
         return 0;
    }
