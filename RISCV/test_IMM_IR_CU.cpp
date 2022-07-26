#include<systemc.h>
#include "IMM_IR_CU.h"
#include "trace.h"

sc_trace_file *wf;

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool> RST{"RST"};
    sc_signal<bool>   clock;
    sc_signal<sc_lv<32>>     Value_from_IMEM{"Value_from_IMEM"};
    sc_signal<sc_lv<1>>      memBusy{"memBusy"};

    sc_signal<sc_lv<32>>    I_imm{"I_imm"};
    sc_signal<sc_lv<32>>    S_imm{"S_imm"};
    sc_signal<sc_lv<32>>    U_imm{"U_imm"};
    sc_signal<sc_lv<32>>    B_imm{"B_imm"};
    sc_signal<sc_lv<32>>    J_imm{"J_imm"};
    sc_signal<sc_lv<5>>     rs1{"rs1"};
    sc_signal<sc_lv<5>>     rs2{"rs2"};
    sc_signal<sc_lv<5>>     rd{"rd"};
    sc_signal<sc_lv<3>>     func3{"func3"};

    sc_signal<sc_lv<1>>    sel1PC{"sel1PC"};
    sc_signal<sc_lv<1>>    sel2PC{"sel2PC"};
    sc_signal<sc_lv<1>>    iPC{"iPC"};
    sc_signal<sc_lv<1>>    wRD{"wRD"};
    sc_signal<sc_lv<1>>    selRD{"selRD"};
    sc_signal<sc_lv<1>>    sel1ALU{"sel1ALU"};
    sc_signal<sc_lv<2>>    sel2ALU{"sel2ALU"};
    sc_signal<sc_lv<4>>    selopALU{"selopALU"};
    sc_signal<sc_lv<1>>    wIR{"wIR"};
    sc_signal<sc_lv<1>>    RDMEM{"RDMEM"};
    sc_signal<sc_lv<1>>    WRMEM{"WRMEM"};
    sc_signal<sc_lv<1>>    IDMEM{"IDMEM"};

    scIMM_IR_CU IMM_IR_CU {"IMM_IR_CU"};

    IMM_IR_CU.clock(clock);
    IMM_IR_CU.RST(RST);
    IMM_IR_CU.Value_from_IMEM(Value_from_IMEM);
    IMM_IR_CU.memBusy(memBusy);
    IMM_IR_CU.I_imm(I_imm);
    IMM_IR_CU.S_imm(S_imm);
    IMM_IR_CU.U_imm(U_imm);
    IMM_IR_CU.B_imm(B_imm);
    IMM_IR_CU.J_imm(J_imm);
    IMM_IR_CU.rs1(rs1);
    IMM_IR_CU.rs2(rs2);
    IMM_IR_CU.rd(rd);
    IMM_IR_CU.func3(func3);
    IMM_IR_CU.sel1PC(sel1PC);
    IMM_IR_CU.sel2PC(sel2PC);
    IMM_IR_CU.iPC(iPC);
    IMM_IR_CU.wRD(wRD);
    IMM_IR_CU.selRD(selRD);
    IMM_IR_CU.sel1ALU(sel1ALU);
    IMM_IR_CU.sel2ALU(sel2ALU);
    IMM_IR_CU.selopALU(selopALU);
    IMM_IR_CU.wIR(wIR);
    IMM_IR_CU.RDMEM(RDMEM);
    IMM_IR_CU.WRMEM(WRMEM);
    IMM_IR_CU.IDMEM(IDMEM);

    sc_start();
    wf= sc_create_vcd_trace_file("test_IMM_IR_CU");

    sc_trace(wf,clock,"clock");
    sc_trace(wf,RST,"RST");
    sc_trace(wf,Value_from_IMEM,"Value_from_IMEM");
    sc_trace(wf,memBusy,"memBusy");

    sc_trace(wf,I_imm,"I_imm");
    sc_trace(wf,S_imm,"S_imm");
    sc_trace(wf,U_imm,"U_imm");
    sc_trace(wf,B_imm,"B_imm");
    sc_trace(wf,J_imm,"J_imm");
    sc_trace(wf,rs1,"rs1");
    sc_trace(wf,rs2,"rs2");
    sc_trace(wf,rd,"rd");
    sc_trace(wf,func3,"func3");

    sc_trace(wf,sel1PC,"sel1PC");
    sc_trace(wf,sel2PC,"sel2PC");
    sc_trace(wf,iPC,"iPC");
    sc_trace(wf,wRD,"wRD");
    sc_trace(wf,selRD,"selRD");
    sc_trace(wf,sel1ALU,"sel1ALU");
    sc_trace(wf,sel2ALU,"sel2ALU");
    sc_trace(wf,selopALU,"selopALU");
    sc_trace(wf,wIR,"wIR");
    sc_trace(wf,RDMEM,"RDMEM");
    sc_trace(wf,WRMEM,"WRMEM");
    sc_trace(wf,IDMEM,"IDMEM");

    RST.write(0);   Value_from_IMEM.write(0);   memBusy.write(0);
    sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

    RST.write(0);   Value_from_IMEM.write(0x1);   memBusy.write(1);
    sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

    RST.write(1);   Value_from_IMEM.write(0x1);   memBusy.write(1);
    sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

    RST.write(0);   Value_from_IMEM.write(0x0FF0000F);   memBusy.write(1);
    sc_start(10, SC_NS); clock=0; sc_start(10, SC_NS); clock =1;

    sc_close_vcd_trace_file(wf);
    return 0;
}
