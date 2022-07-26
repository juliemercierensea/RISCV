#ifndef IMM_IR_CU_H
#define IMM_IR_CU_H

#include <systemc.h>

#include "IMMEDIATE.h"
#include "Control_Unit.h"
#include "IR.h"
#include "fetching_size.h"
#include "trace.h"

SC_MODULE(scIMM_IR_CU){
    sc_in_clk            clock{"clock"};
    sc_in<sc_lv<32>>     Value_from_IMEM{"Value_from_IMEM"};
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

    sc_out<sc_lv<1>>    sel1PC{"sel1PC"};
    sc_out<sc_lv<1>>    sel2PC{"sel2PC"};
    sc_out<sc_lv<1>>    iPC{"iPC"};
    sc_out<sc_lv<1>>    wRD{"wRD"};
    sc_out<sc_lv<1>>    selRD{"selRD"};
    sc_out<sc_lv<1>>    sel1ALU{"sel1ALU"};
    sc_out<sc_lv<2>>    sel2ALU{"sel2ALU"};
    sc_out<sc_lv<4>>    selopALU{"selopALU"};
    sc_out<sc_lv<1>>    wIR{"wIR"};
    sc_out<sc_lv<1>>    RDMEM{"RDMEM"};
    sc_out<sc_lv<1>>    WRMEM{"WRMEM"};
    sc_out<sc_lv<1>>    IDMEM{"IDMEM"};
    sc_out<sc_lv<2>>    size_fromCU{"size_fromCU"};

    scControl_Unit  Control_Unit{"ControlUnit"};
    IR              IR {"IR"};
    scIMMEDIATE     IMM {"IMM"};
    fetch_size      fetchsize{"fetch_size"};

    sc_signal<sc_lv<32>>    RI_value{"RI_value"};
    sc_signal<sc_lv<1>>     fetching{"fetching"};

    SC_CTOR(scIMM_IR_CU){

        fetchsize.size(size_fromCU);
        fetchsize.RI(RI_value);
        fetchsize.fetching(fetching);

        IR.Val(RI_value);
        IR.LoadVal(Value_from_IMEM);
        IR.clock(clock);
        IR.wIR(wIR);
        IR.memBusy(memBusy);

        IMM.RI(RI_value);
        IMM.I(I_imm);
        IMM.S(S_imm);
        IMM.B(B_imm);
        IMM.J(J_imm);
        IMM.U(U_imm);

        Control_Unit.clock(clock);
        Control_Unit.RST(RST);
        Control_Unit.instruction(Value_from_IMEM);
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
        Control_Unit.fetching(fetching);

        SC_THREAD(select);
        sensitive<<RI_value;

        wf= sc_create_vcd_trace_file("itest_IMM_IR_CU");

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

        sc_trace(wf,RI_value,"RI_value");
        sc_trace(wf,fetching,"fetching");
    }

    void select(){
        while(1){
            func3.write((RI_value.read()&0b111000000000000)>>0xC);
            rs1.write((RI_value.read()&0b11111000000000000000)>>0xF);
            rs2.write((RI_value.read()&0b1111100000000000000000000)>>0x14);
            rd.write((RI_value.read()&0b111110000000)>>0x7);
            wait();
        }
    }
};

#endif // IMM_IR_CU_H
