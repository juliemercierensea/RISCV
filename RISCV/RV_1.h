#ifndef RV_1_H
#define RV_1_H
#include <systemc.h>
#include "mux2to1.h"
#include "Regfile.h"
#include "ALU.h"
#include "PC.h"
#include "Memory_Interface.h"
#include "IMM_IR_CU.h"
#include "trace.h"

SC_MODULE(RV_1){
    sc_in_clk           clock{"clock"};
    sc_in<bool>         RST{"RST"};
    sc_in<sc_lv<1>>     PREADY{"PREADY"};
    sc_in<sc_lv<32>>    PRDATA{"PRDATA"};

    sc_out<sc_lv<32>>   PADDR{"PADDR"};
    sc_out<sc_lv<4>>    PSTRB{"PSTRB"};
    sc_out<sc_lv<32>>   PWDATA{"PWDATA"};
    sc_out<sc_lv<1>>    PWRITE{"PWRITE"};
    sc_out<sc_lv<1>>    PENABLE{"PENABLE"};
    sc_out<sc_lv<1>>    PREQ{"PREQ"};

    scMemory_Interface  MemInt {"Memory_Interface"};
    scIMM_IR_CU         IMM_IR_CU {"IMM_IR_CU"};
    scPC                PC {"PC"};
    scALU               ALU{"ALU"};
    scRegfile           regfile{"regfile"};
    mux2to1<32>         muxRD{"muxRD"};
    mux2to1<32>         muxAddress {"muxAddress"};

    sc_signal<sc_lv<1>>     RDMEM{"RDMEM"};
    sc_signal<sc_lv<1>>     WRMEM{"WRMEM"};
    sc_signal<sc_lv<32>>    Address_to_MEM{"address_to_MEM"};
    sc_signal<sc_lv<3>>     func3{"func3"};
    sc_signal<sc_lv<32>>    Value_to_DMEM{"Value_to_DMEM"};
    sc_signal<sc_lv<32>>    Value_from_DMEM{"Value_from_DMEM"};
    sc_signal<sc_lv<32>>    Value_from_IMEM{"Value_from_IMEM"};
    sc_signal<sc_lv<1>>     sel1PC{"sel1PC"};
    sc_signal<sc_lv<1>>     sel2PC{"sel2PC"};
    sc_signal<sc_lv<1>>     iPC{"iPC"};
    sc_signal<sc_lv<1>>     wRD{"wRD"};
    sc_signal<sc_lv<1>>     selRD{"selRD"};
    sc_signal<sc_lv<1>>     sel1ALU{"sel1ALU"};
    sc_signal<sc_lv<2>>     sel2ALU{"sel2ALU"};
    sc_signal<sc_lv<4>>     selopALU{"selopALU"};
    sc_signal<sc_lv<1>>     wIR{"wIR"};
    sc_signal<sc_lv<1>>     IDMEM{"IDMEM"};
    sc_signal<sc_lv<32>>    I_imm{"I_imm"};
    sc_signal<sc_lv<32>>    S_imm{"S_imm"};
    sc_signal<sc_lv<32>>    U_imm{"U_imm"};
    sc_signal<sc_lv<32>>    B_imm{"B_imm"};
    sc_signal<sc_lv<32>>    J_imm{"J_imm"};
    sc_signal<sc_lv<5>>     rs1{"rs1"};
    sc_signal<sc_lv<5>>     rs2{"rs2"};
    sc_signal<sc_lv<5>>     rd{"rd"};
    sc_signal<sc_lv<32>>    Address_to_IMEM{"Address_to_IMEM"};
    sc_signal<sc_lv<32>>    Address_to_DMEM{"Address_to_DMEM"};
    sc_signal<sc_lv<1>>     memBusy{"memBusy"};
    sc_signal<sc_lv<32>>    rs1_value{"rs1_value"};
    sc_signal<sc_lv<32>>    rd_value{"rd_value"};
    sc_signal<sc_lv<1>>     func3_2{"func3_2"};
    sc_signal<sc_lv<2>>     func3_1to0{"func3_1to0"};

    SC_CTOR(RV_1){

        MemInt.RST(RST);
        MemInt.clock(clock);
        MemInt.PRDATA(PRDATA);
        MemInt.PREADY(PREADY);
        MemInt.rd_i(RDMEM);
        MemInt.wr_i(WRMEM);
        MemInt.addr_i(Address_to_MEM);
        MemInt.size_i(func3_1to0);
        MemInt.unsigned_i(func3_2);
        MemInt.wdata_i(Value_to_DMEM);
        MemInt.PADDR(PADDR);
        MemInt.PSTRB(PSTRB);
        MemInt.PWDATA(PWDATA);
        MemInt.PWRITE(PWRITE);
        MemInt.PENABLE(PENABLE);
        MemInt.PREQ(PREQ);
        MemInt.busy_o(memBusy);
        MemInt.rdata_o(Value_from_DMEM);
        MemInt.rdata_unbuff_o(Value_from_IMEM);

        IMM_IR_CU.clock(clock);
        IMM_IR_CU.Value_from_IMEM(Value_from_IMEM);
        IMM_IR_CU.memBusy(memBusy);
        IMM_IR_CU.RST(RST);
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

        PC.rs1_value(rs1_value);
        PC.B_imm(B_imm);
        PC.J_imm(J_imm);
        PC.sel1PC(sel1PC);
        PC.sel2PC(sel2PC);
        PC.iPC(iPC);
        PC.RST(RST);
        PC.clock(clock);
        PC.PC_value(Address_to_IMEM);

        ALU.selopALU(selopALU);
        ALU.sel2ALU(sel2ALU);
        ALU.sel1ALU(sel1ALU);
        ALU.rs1_value(rs1_value);
        ALU.PC_value(Address_to_IMEM);
        ALU.rs2_value(Value_to_DMEM);
        ALU.I_imm(I_imm);
        ALU.U_imm(U_imm);
        ALU.S_imm(S_imm);
        ALU.ALU_value(Address_to_DMEM);

        regfile.clock(clock);
        regfile.reset(RST);
        regfile.WAdd(rd);
        regfile.WVal(rd_value);
        regfile.WCmd(wRD);
        regfile.R0Add(rs1);
        regfile.R1Add(rs2);
        regfile.R0Val(rs1_value);
        regfile.R1Val(Value_to_DMEM);

        muxAddress.i0(Address_to_IMEM);
        muxAddress.i1(Address_to_DMEM);
        muxAddress.sel(IDMEM);
        muxAddress.res(Address_to_MEM);

        muxRD.i0(Address_to_DMEM);
        muxRD.i1(Value_from_DMEM);
        muxRD.sel(selRD);
        muxRD.res(rd_value);

        SC_THREAD(select);
        sensitive<<func3;

        wf= sc_create_vcd_trace_file("itest_RV_1");
        sc_trace(wf,clock,"clock");
        sc_trace(wf,RST,"RST");
        sc_trace(wf,PADDR,"PADDR");
        sc_trace(wf,PSTRB,"PSTRB");
        sc_trace(wf,PENABLE,"PENABLE");
        sc_trace(wf,PREQ,"PREQ");
        sc_trace(wf,PRDATA,"PRDATA");
        sc_trace(wf,PREADY,"PREADY");

        sc_trace(wf,RDMEM,"RDMEM");
        sc_trace(wf,WRMEM,"WRMEM");
        sc_trace(wf,Address_to_MEM,"Address_to_MEM");
        sc_trace(wf,func3,"func3");
        sc_trace(wf,Value_to_DMEM,"Value_to_DMEM");
        sc_trace(wf,Value_from_IMEM,"Value_from_IMEM");
        sc_trace(wf,sel1PC,"sel1PC");
        sc_trace(wf,sel2PC,"sel2PC");
        sc_trace(wf,iPC,"iPC");
        sc_trace(wf,wIR,"wIR");
        sc_trace(wf,IDMEM,"IDMEM");
        sc_trace(wf,I_imm,"I_imm");
        sc_trace(wf,S_imm,"S_imm");
        sc_trace(wf,U_imm,"U_imm");
        sc_trace(wf,B_imm,"B_imm");
        sc_trace(wf,J_imm,"J_imm");
        sc_trace(wf,rs1,"rs1");
        sc_trace(wf,rs2,"rs2");
        sc_trace(wf,rd_value,"rd_value");
        sc_trace(wf,Address_to_IMEM,"Address_to_IMEM");
        sc_trace(wf,Address_to_DMEM,"Address_to_DMEM");
        sc_trace(wf,memBusy,"memBusy");
        sc_trace(wf,rs1_value,"rs1_value");
        sc_trace(wf,rd_value,"rd_value");
        sc_trace(wf,func3_2,"func3_2");
        sc_trace(wf,func3_1to0,"func3_1to0");

    }

    void select(){
        while(1){
            func3_1to0.write(func3.read()&0b11);
            func3_2.write((func3.read()&0b100)>>0x2);
            wait();
        }
    }

};

#endif // RV_1_H
