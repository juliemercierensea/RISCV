#ifndef IMM_IR_CU_H
#define IMM_IR_CU_H

#include <systemc.h>

#include "IMMEDIATE.h"
#include "Control_Unit.h"
#include "IR.h"
#include "SelRI_value.h"

SC_MODULE(scIMM_IR_CU){
    sc_in_clk            clock{"clock"};
    sc_in<sc_lv<32>>     rdata_unbuff_o{"rdata_unbuff_o"};
    sc_in<sc_lv<1>>      memBusy{"memBusy"};
    sc_in<bool>          RST{"RST"};

    sc_out<sc_lv<32>>    I_imm{"I_imm"};
    sc_out<sc_lv<32>>    S_imm{"S_imm"};
    sc_out<sc_lv<32>>    U_imm{"U_imm"};
    sc_out<sc_lv<32>>    B_imm{"B_imm"};
    sc_out<sc_lv<32>>    J_imm{"J_imm"};
    sc_out<sc_lv<5>>     rs1{"rs1"};
    sc_out<sc_lv<5>>     rs2{"rs2"};
    sc_out<sc_lv<5>>     rd{"rd"};
    sc_out<sc_lv<3>>     func3{"func3"};

    sc_out<sc_lv<1>>    sel1PC;
    sc_out<sc_lv<1>>    sel2PC;
    sc_out<sc_lv<1>>    iPC;
    sc_out<sc_lv<1>>    wRD;
    sc_out<sc_lv<1>>    selRD;
    sc_out<sc_lv<1>>    sel1ALU;
    sc_out<sc_lv<2>>    sel2ALU;
    sc_out<sc_lv<4>>    selopALU;
    sc_out<sc_lv<1>>    wIR{"wIR"};
    sc_out<sc_lv<1>>    RDMEM;
    sc_out<sc_lv<1>>    WRMEM;
    sc_out<sc_lv<1>>    IDMEM;

    scControl_Unit  Control_Unit{"ControlUnit"};
    IR              IR {"IR"};
    scIMMEDIATE     IMM {"IMM"};
    scSelRI         selRI {"selRI"};

    sc_signal<sc_lv<32>>    RI_value{"RI_value"};


    SC_CTOR(scIMM_IR_CU){
        IR.Val(RI_value);
        IR.LoadVal(rdata_unbuff_o);
        IR.clock(clock);
        IR.wIR(wIR);
        IR.memBusy(memBusy);

        IMM.RI(RI_value);
        IMM.I(I_imm);
        IMM.S(S_imm);
        IMM.B(B_imm);
        IMM.J(J_imm);
        IMM.U(U_imm);

        selRI.RI(RI_value);
        selRI.rs1(rs1);
        selRI.rs2(rs2);
        selRI.rd(rd);
        selRI.func3(func3);

        Control_Unit.clock(clock);
        Control_Unit.RST(RST);
        Control_Unit.instruction(rdata_unbuff_o);
        Control_Unit.memBusy(memBusy);
        Control_Unit.sel1PC(sel1PC);
        Control_Unit.sel2PC(sel2PC);
        Control_Unit.iPC(iPC);
        Control_Unit.wRD(wRD);
        Control_Unit.selRD(selRD);
        Control_Unit.sel1ALU(sel1ALU);
        Control_Unit.sel2ALU(sel2ALU);
        Control_Unit.selopALU(selopALU);
        Control_Unit.wIR(wIR);
        Control_Unit.RD(RDMEM);
        Control_Unit.WR(WRMEM);
        Control_Unit.IDMEM(IDMEM);
    }
};

#endif // IMM_IR_CU_H
