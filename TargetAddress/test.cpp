#include <systemc.h>
#include "mux1PC.h"
#include "mux2PC.h"
#include "PC.h"
#include "TargetAdd.h"
#include "WritePC.h"


int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<sc_lv<32>> targetPC{"LoadVal_targetPC_i"};
    sc_signal<sc_lv<32>> wPC{"Load_wPC_i"};
    sc_signal<sc_lv<32>> iPC("Inc");
    sc_signal<sc_lv<32>> PC_value{"Count_o"};
    sc_signal<bool> RST("reset");
    sc_signal<bool>   clock;

    sc_signal<sc_lv<3>> flags("flags");
    sc_signal<bool> sel1("sel_mux1");
    sc_signal<sc_lv<2>> sel2("sel_mux2");
    sc_signal<sc_lv<32>> rs1("rs1_value");
    sc_signal<sc_lv<32>> rs2("rs2_value");
    sc_signal<sc_lv<32>> I_imm("I_imm");
    sc_signal<sc_lv<32>> U_imm("U_imm");
    sc_signal<sc_lv<32>> S_imm("S_imm");

    scPC PC("RISCV_PC");
    mux1PC mux1PC ("mux1PC");
    mux2PC mux2PC ("mux2PC");
    scTargetAdd TargetAdd ("RISCV_Target_Address");
    scWritePC WritePC ("RISCV_WritePC");

    PC.

    ALU.selop(selop);
    ALU.op1(op1);
    ALU.op2(op2);
    ALU.res(res);
    ALU.flags(flags);

    mux1ALU.sel(sel1);
    mux1ALU.i1(PC_value);
    mux1ALU.i0(rs1);
    mux1ALU.res(op1);

    mux2ALU.sel(sel2);
    mux2ALU.i0(rs2);
    mux2ALU.i1(I_imm);
    mux2ALU.i2(U_imm);
    mux2ALU.i3(S_imm);
    mux2ALU.res(op2);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("ALU");

     sc_trace(wf,selop,"opsel");
     sc_trace(wf,op1,"op1");
     sc_trace(wf,op2,"op2");
     sc_trace(wf,res,"res");
     sc_trace(wf,sel1,"sel1");
     sc_trace(wf,sel2,"sel2");
     sc_trace(wf,rs2,"rs2");
     sc_trace(wf,I_imm,"I_imm");
     sc_trace(wf,U_imm,"U_imm");
     sc_trace(wf,S_imm,"S_imm");
     sc_trace(wf,rs1,"rs1");
     sc_trace(wf,PC_value,"PC_value");
     //sc_trace(wf,flags,"flags");

     sel1.write(1);
     sel2.write(1);
     rs2.write(1);
     I_imm.write(2147483650);
     U_imm.write(3);
     S_imm.write(4);
     rs1.write(5);
     PC_value.write(101);
     op1.write(10);
     op2.write(2);

     sc_close_vcd_trace_file(wf);
     return 0;
}
