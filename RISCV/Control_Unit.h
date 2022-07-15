#ifndef CONTROL_UNIT_H
#define CONTROL_UNIT_H

#include <systemc.h>
#include <AddressCounter.h>
#include <InstrNumber.h>
#include <mux2to1.h>
#include <Instruction.h>
#include <ROMmem.h>
#include <ROMData_to_input.h>

SC_MODULE (scControl_Unit) {
    sc_in<sc_lv<32>>    instruction;
    sc_in<sc_lv<1>>     memBusy;
    sc_in_clk           clock;
    sc_in<bool>         RST;

    sc_out<sc_lv<1>>    sel1PC;
    sc_out<sc_lv<1>>    sel2PC;
    sc_out<sc_lv<1>>    iPC;
    sc_out<sc_lv<1>>    wRD;
    sc_out<sc_lv<1>>    selRD;
    sc_out<sc_lv<1>>    sel1ALU;
    sc_out<sc_lv<2>>    sel2ALU;
    sc_out<sc_lv<4>>    selopALU;
    sc_out<sc_lv<1>>    wIR;
    sc_out<sc_lv<1>>    RD;
    sc_out<sc_lv<1>>    WR;
    sc_out<sc_lv<1>>    IDMEM;

    scAddressCounter    AddCounter{"RISCV_AddressCounter"};
    scInstrNumber       InstrNb {"Instruction_Number"};
    scInstruction       InstrSel{"Instruction_Selection"};
    mux2to1<8>          muxEOI {"Mux_EOI"};
    mux2to1<8>          muxEOF {"Mux_EOF"};
    scROM_Memory        ROMmem {"ROM_Memory"};
    scSelectfromROMData selROMData {"SelectfromROMData"};

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
    sc_signal<sc_lv<32>> rdata_unbuff_o{"rdata_unbuff_o"};
    sc_signal<sc_lv<1>>  wr_i{"wr_i"};
    sc_signal<sc_lv<1>>  rd_i{"rd_i"};

    /*sc_signal<sc_lv<1>>  IDMEM{"IDMEM"};
    sc_signal<sc_lv<1>>  wIR{"wIR"};
    sc_signal<sc_lv<1>>  wRD{"wRD"};
    sc_signal<sc_lv<1>>  selRD{"selRD"};
    sc_signal<sc_lv<1>>  sel1ALU{"sel1ALU_mux1"};
    sc_signal<sc_lv<2>>  sel2ALU{"sel2ALU_mux2"};
    sc_signal<sc_lv<4>>  selopALU{"selopALU"};*/


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

        InstrSel.instruction(rdata_unbuff_o);
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
        ROMmem.Data(uINSTR);
        ROMmem.clock(clock);

        selROMData.ROMData(uINSTR);
        selROMData.EOF_o(EOF_i);
        selROMData.EOI_o(EOI);
        selROMData.waitMEM_o(waitMEM);
        selROMData.IDMEM(IDMEM);
        selROMData.RD(rd_i);
        selROMData.WR(wr_i);
        selROMData.wIR(wIR);
        selROMData.iPC(iPC);
        selROMData.selRD(selRD);
        selROMData.wRD(wRD);
        selROMData.selopALU(selopALU);
        selROMData.sel2ALU(sel2ALU);
        selROMData.sel1ALU(sel1ALU);
        selROMData.sel2PC(sel2PC);
        selROMData.sel1PC(sel1PC);
    }


};


#endif // CONTROL_UNIT_H
