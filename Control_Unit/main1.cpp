/*!
 * \file main.cpp
 * \brief main function
 * \author Julie Mercier
 * \version 0.1
 * \date 13 May 2022
 *
 * simulate and link the signals to the ports, used as a testbench for the global control unit block\n
 */

#include <systemc.h>
#include "AddressCounter.h"
#include "InstrNumber.h"
#include "Instruction.h"
#include "MuxEOF.h"
#include "MuxEOI.h"
#include "ROMmem.h"
#include "mux2to1.h"

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>   clock;
    sc_signal<bool> EOI;
    sc_signal<bool> EOF_i;
    sc_signal<bool> RST("reset");
    sc_signal<bool> waitMEM;
    sc_signal<bool> memBusy;
    sc_signal<sc_lv<8>> INUM;
    sc_signal<sc_lv<8>> uCYCLE;

    sc_signal<bool> func12_0;
    sc_signal<bool> func7_5;
    sc_signal<sc_lv<3>>  func3;
    sc_signal<sc_lv<5>>  opcode_6to2;

    sc_signal<sc_lv<32>> instruction;

    sc_signal<sc_lv<8>>  EOFmtoEOIm;
    sc_signal<sc_lv<8>>  EOI_0x0;

    sc_signal<sc_lv<8>>  uADDR {"Address"};
    sc_signal<sc_lv<32>> uINSTR;


    scAddressCounter AddCounter("RISCV_AddressCounter");
    scInstrNumber InstrNb ("Instruction_Number");
    scInstruction InstrSel("Instruction_Selection");
    mux2to1 muxEOI ("Mux_EOI");
    mux2to1 muxEOF ("Mux_EOF");
    scROM_Memory ROMmem ("ROM_Memory");

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
    ROMmem.Data(uINSTR);
    ROMmem.clock(clock);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("ControlUnit_tbCUSTOM1enable");

     sc_trace(wf,clock,"clock");
     sc_trace(wf,EOF_i,"EOF_i");
     sc_trace(wf,EOI,"EOI");
     //sc_trace(wf,waitMEM,"waitMEM");
     //sc_trace(wf,memBusy,"memBusy");
     sc_trace(wf,uCYCLE,"uCYCLE");
     sc_trace(wf,INUM,"INUM");
     //sc_trace(wf,RST,"reset");

     sc_trace(wf,func12_0,"func12_0");
     sc_trace(wf,func7_5,"func7_5");
     sc_trace(wf,func3,"func3");
     sc_trace(wf,opcode_6to2,"opcode_6to2");

     sc_trace(wf,instruction,"instruction");

     sc_trace(wf,EOFmtoEOIm,"EOFmtoEOIm");

     //sc_trace(wf,EOI_0x0,"EOI_0x0");

     sc_trace(wf,uADDR,"uADDR");
     sc_trace(wf,uINSTR,"uINSTR");

     instruction.write(0b111111111000000000111110101000);
     waitMEM.write(0);
     memBusy.write(0);
     RST.write(0);

     for (int i=0;i<1;i++) {
         EOI.write(1);
         EOF_i.write(1);

        clock = 0;
        sc_start(10, SC_NS);
        clock = 1;
        sc_start(10, SC_NS);
     }

     for (int i=0;i<1;i++) {
         EOI.write(0);
         EOF_i.write(1);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);
     }

     for (int i=0;i<1;i++) {
         EOI.write(0);
         EOF_i.write(0);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);


     }

     for (int i=0;i<1;i++) {
         EOI.write(1);
         EOF_i.write(0);
         clock = 0;
         sc_start(10, SC_NS);
         clock = 1;
         sc_start(10, SC_NS);

     }

     sc_close_vcd_trace_file(wf);
     return 0;
}
