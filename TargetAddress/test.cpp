#include <systemc.h>
#include "mux2to1.h"
#include "PC.h"
#include "TargetAdd.h"
#include "WritePC.h"


int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<sc_lv<32>> targetPC{"LoadVal_targetPC_i"};
    sc_signal<sc_lv<32>> wPC{"Load_wPC_i"};
    sc_signal<bool> iPC("Inc");
    sc_signal<bool> RST("reset");
    sc_signal<bool>   clock;
    sc_signal<sc_lv<32>>    mux1_to_targetAdd;
    sc_signal<sc_lv<32>>    mux2_to_targetAdd;

    sc_signal<bool> sel1PC("sel_mux1");
    sc_signal<bool> sel2PC("sel_mux2");
    sc_signal<sc_lv<32>> rs1("rs1_value");
    sc_signal<sc_lv<32>> PC_value{"Count_o"};
    sc_signal<sc_lv<32>> B_imm("B_imm");
    sc_signal<sc_lv<32>> J_imm("J_imm");

    scPC PC("RISCV_PC");
    mux2to1 mux1PC("mux1PC");
    mux2to1 mux2PC("mux2PC");
    scTargetAdd TargetAdd ("RISCV_Target_Address");

    mux1PC.sel(sel1PC);
    mux1PC.i1(rs1);
    mux1PC.i0(PC_value);
    mux1PC.res(mux1_to_targetAdd);

    mux2PC.sel(sel2PC);
    mux2PC.i0(B_imm);
    mux2PC.i1(J_imm);
    mux2PC.res(mux2_to_targetAdd);

    PC.LoadVal(targetPC);
    PC.Load(wPC);
    PC.Inc(iPC);
    PC.Count(PC_value);
    PC.clock(clock);
    PC.RST(RST);

    TargetAdd.op1(mux1_to_targetAdd);
    TargetAdd.op2(mux2_to_targetAdd);
    TargetAdd.targetPC(targetPC);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("RegPC_tb");

     sc_trace(wf,sel1PC,"sel1PC");
     sc_trace(wf,sel2PC,"sel2PC");
     sc_trace(wf,PC_value,"PC_value");
     sc_trace(wf,rs1,"rs1");
     sc_trace(wf,B_imm,"B_imm");
     sc_trace(wf,J_imm,"J_imm");
     sc_trace(wf,targetPC,"targetPC");
     sc_trace(wf,mux1_to_targetAdd,"mux1_to_targetAdd");
     sc_trace(wf,mux2_to_targetAdd,"mux2_to_targetAdd");
     sc_trace(wf,wPC,"wPC");
     sc_trace(wf,iPC,"iPC");
     sc_trace(wf,RST,"reset");
     sc_trace(wf,clock,"clock");

     sel1PC.write(0);
     sel2PC.write(1);
     rs1.write(1);
     B_imm.write(2);
     J_imm.write(3);

     for(int i=0; i<2;i++){
         RST.write(0);
         iPC.write(1);
         wPC.write(0);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);

     }
     for(int i=0; i<2;i++){
         RST.write(0);
         iPC.write(1);
         wPC.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);

     }
     for(int i=0; i<2;i++){
         RST.write(0);
         iPC.write(0);
         wPC.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);

     }
     for(int i=0; i<1;i++){
         iPC.write(0);
         wPC.write(1);
         RST.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);

     }

     sc_close_vcd_trace_file(wf);
     return 0;
}
