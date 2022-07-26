#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <systemc.h>
#include <AddressCounter.h>
#include <InstrNumber.h>
#include <mux2to1.h>
#include <Instruction.h>
#include <ROMmem.h>
#include <ROMData_to_input.h>
#include "trace.h"
#include "iPC_Control.h"

SC_MODULE (scControl_Unit) {
    sc_in<sc_lv<32>>    instruction{"instruction"};
    sc_in<sc_lv<1>>     memBusy{"memBusy"};
    sc_in_clk           clock{"clock"};
    sc_in<bool>         RST{"reset"};

    sc_out<sc_lv<1>>    sel1PC{"sel1PC"};
    sc_out<sc_lv<1>>    sel2PC{"sel2PC"};
    sc_out<sc_lv<1>>    iPC{"iPC"};
    sc_out<sc_lv<1>>    wRD{"wRD"};
    sc_out<sc_lv<1>>    selRD{"selRD"};
    sc_out<sc_lv<1>>    sel1ALU{"sel1ALU"};
    sc_out<sc_lv<2>>    sel2ALU{"sel2ALU"};
    sc_out<sc_lv<4>>    selopALU{"selopALU"};
    sc_out<sc_lv<1>>    wIR{"wIR"};
    sc_out<sc_lv<1>>    RD{"RD"};
    sc_out<sc_lv<1>>    WR{"WR"};
    sc_out<sc_lv<1>>    IDMEM{"IDMEM"};
    sc_out<sc_lv<1>>    fetching{"fetching"};

    scAddressCounter    AddCounter{"RISCV_AddressCounter"};
    scInstrNumber       InstrNb {"Instruction_Number"};
    scInstruction       InstrSel{"Instruction_Selection"};
    mux2to1<8>          muxEOI {"Mux_EOI"};
    mux2to1<8>          muxEOF {"Mux_EOF"};
    scROM_Memory        ROMmem {"rvucrom.xml","ROM_Memory"};
    scSelectfromROMData selROMData {"SelectfromROMData"};
    iPC_Control     iPC_control{"iPC_control"};

    sc_signal<sc_lv<8>>  INUM{"INUM"};
    sc_signal<bool>      func12_0{"func12_0"};
    sc_signal<bool>      func7_5{"func7_5"};
    sc_signal<sc_lv<3>>  func3{"func3"};
    sc_signal<sc_lv<5>>  opcode_6to2{"opcode_6to2"};
    sc_signal<sc_lv<1>>  EOI{"EOI"};
    sc_signal<sc_lv<1>>  EOF_i{"EOF_i"};
    sc_signal<sc_lv<1>>  waitMEM{"waitMEM"};
    sc_signal<sc_lv<8>>  uCYCLE{"uCYCLE"};
    sc_signal<sc_lv<8>>  EOFmtoEOIm{"EOFmtoEOIm"};
    sc_signal<sc_lv<8>>  EOI_0x0{"EOI_0x0"};
    sc_signal<sc_lv<8>>  uADDR {"Address"};
    sc_signal<sc_lv<32>> uINSTR{"uINSTR"};
    sc_signal<sc_lv<1>> iPC_i{"iPC_i"};

    SC_CTOR(scControl_Unit) {

        AddCounter.EOF_i(EOF_i);
        AddCounter.EOI(EOI);
        AddCounter.waitMEM(waitMEM);
        AddCounter.memBusy(memBusy);
        AddCounter.Val(uCYCLE);
        AddCounter.LoadVal(INUM);
        AddCounter.clock(clock);
        AddCounter.RST(RST);
        AddCounter.EOI_i0(EOI_0x0);

        InstrNb.func12_0(func12_0);
        InstrNb.func7_5(func7_5);
        InstrNb.func3(func3);
        InstrNb.opcode_6to2(opcode_6to2);
        InstrNb.number(INUM);

        InstrSel.instruction(instruction);
        InstrSel.func12_0(func12_0);
        InstrSel.func7_5(func7_5);
        InstrSel.func3(func3);
        InstrSel.opcode_6to2(opcode_6to2);

        muxEOF.sel(EOF_i);
        muxEOF.i1(INUM);
        muxEOF.i0(uCYCLE);
        muxEOF.res(EOFmtoEOIm);

        muxEOI.sel(EOI);
        muxEOI.i1(EOI_0x0);
        muxEOI.i0(EOFmtoEOIm);
        muxEOI.res(uADDR);

        ROMmem.Address(uADDR);
        ROMmem.waitMEM(waitMEM);
        ROMmem.memBusy(memBusy);
        ROMmem.ROMData(uINSTR);
        ROMmem.clock(clock);
        ROMmem.RST(RST);

        selROMData.ROMData(uINSTR);
        selROMData.EOF_o(EOF_i);
        selROMData.EOI_o(EOI);
        selROMData.waitMEM_o(waitMEM);
        selROMData.IDMEM(IDMEM);
        selROMData.RD(RD);
        selROMData.WR(WR);
        selROMData.wIR(wIR);
        selROMData.iPC(iPC_i);
        selROMData.selRD(selRD);
        selROMData.wRD(wRD);
        selROMData.selopALU(selopALU);
        selROMData.sel2ALU(sel2ALU);
        selROMData.sel1ALU(sel1ALU);
        selROMData.sel2PC(sel2PC);
        selROMData.sel1PC(sel1PC);
        selROMData.fetching(fetching);

        iPC_control.waitMem(waitMEM);
        iPC_control.memBusy(memBusy);
        iPC_control.iPC_i(iPC_i);
        iPC_control.iPC_o(iPC);

        wf= sc_create_vcd_trace_file("itest_ControlUnit");

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

        sc_trace(wf,INUM,"INUM");
        sc_trace(wf,func12_0,"func12_0");
        sc_trace(wf,func7_5,"func7_5");
        sc_trace(wf,func3,"func3");
        sc_trace(wf,opcode_6to2,"opcode_6to2");
        sc_trace(wf,EOI,"EOI");
        sc_trace(wf,EOF_i,"EOF_i");
        sc_trace(wf,waitMEM,"waitMEM");
        sc_trace(wf,uCYCLE,"uCYCLE");
        sc_trace(wf,EOFmtoEOIm,"EOFmtoEOIm");
        sc_trace(wf,EOI_0x0,"EOI_0x0");
        sc_trace(wf,uADDR,"uADDR");
        sc_trace(wf,uINSTR,"uINSTR");
        sc_trace(wf,iPC_i,"iPC_i");
        sc_trace(wf,fetching,"fetching");
    }
};


#endif // CONTROL_UNIT_H
