#include<systemc.h>
#include "mux2to1.h"
#include "mux3to1.h"
#include "mux4to1.h"
#include "ALU.h"
#include "Regfile.h"
#include "IR.h"
#include "IMMEDIATE.h"
#include "AddressCounter.h"
#include "InstrNumber.h"
#include "Instruction.h"
#include "ROMmem.h"
#include "ROMData_to_input.h"
#include "PC.h"
#include "TargetAdd.h"
#include "WritePC.h"
#include "SelRI_value.h"

#include "MI_StateMachine.h"
#include "MI_StateOutput.h"
//#include "MI_IMEM_contents.h"
#include "MI_registre.h"
#include "MI_size_strobes.h"
#include "MI_shift_left.h"
#include "MI_shift_right.h"
#include "IHex.h"
#include "MI_select_bits.h"
#include "MI_ORbytestrb.h"
#include "MI_zeroextend.h"
#include "MI_byte_extend.h"
#include "MI_half_extend.h"
#include "MI_plus_PADDR.h"
#include "MI_ORtrigger.h"
#include "MI_AND.h"

#include "AMBA_SlaveDecoder.h"
#include "AMBA_master.h"
#include "AMBA_Slave_InstructionMem.h"
#include "AMBA_Slave1_dataMEM.h"

#include "MI_CPUside.h"

int sc_main (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>     clock("clock");
    sc_signal<bool>     RST("reset");

    sc_signal<sc_lv<1>> EOI("EOI");
    sc_signal<sc_lv<1>> EOF_i("EOF_i");
    sc_signal<sc_lv<1>>     waitMEM("waitMEM");
    sc_signal<sc_lv<1>>     memBusy("memBusy");
    sc_signal<sc_lv<8>> INUM("INUM");
    sc_signal<sc_lv<8>> uCYCLE("uCYCLE");

    sc_signal<bool>      func12_0("func12_0");
    sc_signal<bool>      func7_5("func7_5");
    sc_signal<sc_lv<3>>  func3("func3");
    sc_signal<sc_lv<5>>  opcode_6to2("opcode_6to2");
    //sc_signal<sc_lv<32>> instruction("instruction");
    sc_signal<sc_lv<8>>  EOFmtoEOIm("EOFmtoEOIm");
    sc_signal<sc_lv<8>>  EOI_0x0("EOI_0x0");
    sc_signal<sc_lv<8>>  uADDR {"Address"};
    sc_signal<sc_lv<32>> uINSTR{"uINSTR"};

    sc_signal<sc_lv<4>>  selopALU("selopALU");
    sc_signal<sc_lv<32>> muxtoop1("muxtoop1");
    sc_signal<sc_lv<32>> muxtoop2("muxtoop2");
    sc_signal<sc_lv<32>> ALU_value("ALU_value");
    sc_signal<sc_lv<1>>  sel1ALU("sel1ALU_mux1");
    sc_signal<sc_lv<2>>  sel2ALU("sel2ALU_mux2");
    sc_signal<sc_lv<32>> rs1_value("rs1_value");
    sc_signal<sc_lv<32>> PC_value("PC_value");
    sc_signal<sc_lv<32>> rs2_value("rs2_value");
    sc_signal<sc_lv<32>> I_imm("I_imm");
    sc_signal<sc_lv<32>> U_imm("U_imm");
    sc_signal<sc_lv<32>> S_imm("S_imm");
    sc_signal<sc_lv<32>> B_imm("B_imm");
    sc_signal<sc_lv<32>> J_imm("J_imm");
    sc_signal<sc_lv<32>> addr_i("addr_i");
    sc_signal<sc_lv<32>> targetPC{"LoadVal_targetPC_i"};
    sc_signal<sc_lv<32>> wPC{"Load_wPC_i"};
    sc_signal<sc_lv<1>>  iPC("Inc");
    sc_signal<sc_lv<1>>  wRD("wRD");
    sc_signal<sc_lv<1>>  selRD("selRD");
    sc_signal<sc_lv<1>>  IDMEM("IDMEM");
    sc_signal<sc_lv<1>>  wIR("wIR");
    sc_signal<sc_lv<1>>  wr_i("wr_i");
    sc_signal<sc_lv<1>>  rd_i("rd_i");
    sc_signal<sc_lv<32>> mux1_to_targetAdd("mux1_to_targetAdd");
    sc_signal<sc_lv<32>> mux2_to_targetAdd("mux2_to_targetAdd");
    sc_signal<sc_lv<32>>    rdata_unbuff_o("rdata_unbuff_o");

    sc_signal<sc_lv<1>>      sel1PC("sel_mux1");
    sc_signal<sc_lv<1>>      sel2PC("sel_mux2");

    //---------------------------------------- Control Unit -----------------------------------------------------------
    scAddressCounter AddCounter("RISCV_AddressCounter");
    scInstrNumber InstrNb ("Instruction_Number");
    scInstruction InstrSel("Instruction_Selection");
    mux2to1<8> muxEOI ("Mux_EOI");
    mux2to1<8> muxEOF ("Mux_EOF");
    scROM_Memory ROMmem ("ROM_Memory");
    scSelectfromROMData selROMData ("SelectfromROMData");

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


    //---------------------------------------- ALU -----------------------------------------------------------
    scALU ALU("RISCV_ALU");
    mux4to1<32> mux2ALU ("Mux2_ALU");
    mux2to1<32> mux1ALU ("Mux1_ALU");

    ALU.selop(selopALU);
    ALU.op1(muxtoop1);
    ALU.op2(muxtoop2);
    ALU.res(ALU_value);

    mux1ALU.sel(sel1ALU);
    mux1ALU.i1(PC_value);
    mux1ALU.i0(rs1_value);
    mux1ALU.res(muxtoop1);

    mux2ALU.sel(sel2ALU);
    mux2ALU.i0(rs2_value);
    mux2ALU.i1(I_imm);
    mux2ALU.i2(U_imm);
    mux2ALU.i3(S_imm);
    mux2ALU.res(muxtoop2);

    //---------------------------------------- PC -------------------------------------------------------
    scPC PC("RISCV_PC");
    mux2to1<32> mux1PC("mux1PC");
    mux2to1<32> mux2PC("mux2PC");
    scTargetAdd TargetAdd ("RISCV_Target_Address");

    mux1PC.sel(sel1PC);
    mux1PC.i1(rs1_value);
    mux1PC.i0(PC_value);
    mux1PC.res(mux1_to_targetAdd);

    mux2PC.sel(sel2PC);
    mux2PC.i0(B_imm);
    mux2PC.i1(J_imm);
    mux2PC.res(mux2_to_targetAdd);

    PC.LoadVal(targetPC);
    //PC.Load(wPC);
    PC.Inc(iPC);
    PC.Count(PC_value);
    PC.clock(clock);
    PC.RST(RST);

    TargetAdd.op1(mux1_to_targetAdd);
    TargetAdd.op2(mux2_to_targetAdd);
    TargetAdd.targetPC(targetPC);

    //---------------------------------------- REGFILE -----------------------------------------------------------
    sc_signal<sc_lv<5>>  rd("rd");
    sc_signal<sc_lv<32>> rd_value("rd_value");

    sc_signal<sc_lv<5>>  rs1("rs1");
    sc_signal<sc_lv<1>>  rxcommand("rx_command");
    sc_signal<sc_lv<5>>  rs2("rs2");
    sc_signal<sc_lv<1>>  rycommand("rycommand");

    scRegfile REGFILE("RISCV_Regfile");

    REGFILE.clock(clock);
    REGFILE.reset(RST);
    REGFILE.WAdd(rd);
    REGFILE.WVal(rd_value);
    REGFILE.WCmd(wRD);
    REGFILE.R0Add(rs1);
    //REGFILE.rx_command(rxcommand);
    REGFILE.R1Add(rs2);
    //REGFILE.ry_command(rycommand);
    REGFILE.R0Val(rs1_value);
    REGFILE.R1Val(rs2_value);

    //---------------------------------------- muxAddress ------------------------------------------
    mux2to1<32>  muxAddress ("muxAddress");
    muxAddress.i0(PC_value);
    muxAddress.i1(ALU_value);
    muxAddress.res(addr_i);
    muxAddress.sel(IDMEM);

    //---------------------------------------- muxRD ------------------------------------------------------
    mux2to1<32> muxRD("muxRD");
    sc_signal<sc_lv<32>>    rdata_o("rdata_o");
    muxRD.i0(ALU_value);
    muxRD.i1(rdata_o);
    muxRD.sel(selRD);
    muxRD.res(rd_value);

    //---------------------------------------- IMMEDIATE ------------------------------------------------------
    scIMMEDIATE IMMEDIATE ("immediate");
    sc_signal<sc_lv<32>> RI_value("RI_value");
    IMMEDIATE.RI(RI_value);
    IMMEDIATE.I(I_imm);
    IMMEDIATE.S(S_imm);
    IMMEDIATE.U(U_imm);
    IMMEDIATE.B(B_imm);
    IMMEDIATE.J(J_imm);

    //---------------------------------------- IR ------------------------------------------------------
    IR IR("IR");
    IR.LoadVal(rdata_unbuff_o); //Value from IMEM
    IR.Val(RI_value);
    IR.clock(clock);
    IR.wIR(wIR);
    IR.MemBusy(memBusy);

    //---------------------------------------- SelRI_value -----------------------------------------------
    scSelRI selRI("SelRI_value");
    selRI.rd(rd);
    selRI.rs1(rs1);
    selRI.rs2(rs2);
    //selRI.func3(func3);
    selRI.RI(RI_value);

    //---------------------------------------- Memory Interface ---------------------------------------------
    sc_signal<sc_lv<4>>     PSTRB("PSTRB");
    sc_signal<sc_lv<32>>    PWDATA("PWDATA");
    sc_signal<sc_lv<32>>    PRDATA("PRDATA");
    sc_signal<sc_lv<32>>    PRDATA0("PRDATA0");
    sc_signal<sc_lv<32>>    PRDATA1("PRDATA1");
    sc_signal<sc_lv<30>>    PADDR30("PADDR30");
    sc_signal<sc_lv<32>>    PADDR32("PADDR32");
    sc_signal<sc_lv<2>>     PSEL("PSEL");
    sc_signal<sc_lv<1>>     PWRITE("PWRITE");
    sc_signal<sc_lv<1>>     PREQ("PREQ");
    sc_signal<sc_lv<1>>     trigger("trigger");
    sc_signal<sc_lv<1>>     PREADY("PREADY");
    sc_signal<sc_lv<1>>     PREADY0("PREADY0");
    sc_signal<sc_lv<1>>     PREADY1("PREADY1");
    sc_signal<sc_lv<1>>     unaligned("unaligned");
    sc_signal<bool>         condition("condition");
    sc_signal<sc_lv<1>>     unaligned_or_notPREADY("unaligned_or_notPREADY");
    sc_signal<sc_lv<1>>     notPREADY("notPREADY");

    sc_signal<sc_lv<1>>     op1("op1");
    sc_signal<sc_lv<1>>     op2("op2");
    sc_signal<sc_lv<1>>     first_cycle("first_cycle");
    sc_signal<sc_lv<2>>     busy_sel("busy_sel");
    sc_signal<sc_lv<1>>     preq_sel("preq_sel");
    sc_signal<sc_lv<1>>     PENABLE("PENABLE");
    sc_signal<bool>         CurrentState("CurrentState");
    sc_signal<sc_lv<2>>     CS_StatetoStateOutput("CS_StatetoStateOutput");
    sc_signal<sc_lv<2>>     ShowCurrentState("ShowCurrentState");
    sc_signal<sc_lv<1>>     numSlave("numSlave");

    //AMBA

    scIMEM IMEM("IMEM_slave0");
    IMEM.PCLK(clock);
    IMEM.PRSTn(RST);
    IMEM.PADDR(PADDR32); //should be lowXPADDR
    IMEM.PSTRB(PSTRB);
    IMEM.PENABLE(PENABLE);
    IMEM.PSEL(PSEL);
    IMEM.PRDATA(PRDATA0);
    IMEM.PREADY(PREADY0);

    scSlave1 slave1 ("DataMemory_Slave1");
    slave1.PCLK(clock);
    slave1.PRSTn(RST);
    slave1.PADDR(PADDR32); //should be lowYPADDR
    slave1.PSTRB(PSTRB);
    slave1.PWDATA(PWDATA);
    slave1.PWRITE(PWRITE);
    slave1.PENABLE(PENABLE);
    slave1.PSEL(PSEL);

    scDecoder decoder ("decoder");
    sc_signal<sc_lv<8>>  highA_PADDR("highA_PADDR");
    decoder.address(highA_PADDR);
    decoder.enable(PREQ);
    decoder.numSlave(numSlave);
    decoder.sel(PSEL);

    //MUX PRDATA
    mux2to1<32> mux0 ("Mux_0PRDATA");
    mux0.sel(numSlave);
    mux0.i0(PRDATA0);
    mux0.i1(PRDATA1);
    mux0.res(PRDATA);

    //MUXPREADY
    mux2to1<1> mux1 ("Mux_1PREADY");
    mux1.sel(numSlave);
    mux1.i0(PREADY0);
    mux1.i1(PREADY1);
    mux1.res(PREADY);

    //State Machine
    scState State("AMBA_StateMachine");
    State.clock(clock);
    State.RST(RST);
    State.trigger(trigger);
    State.PREADY(PREADY);
    State.unaligned(unaligned);
    State.condition(condition);
    State.CurrentState(CS_StatetoStateOutput);
    State.notPREADY(notPREADY);
    State.unaligned_or_notPREADY(unaligned_or_notPREADY);

    scStateOutput StateOutput("AMBA_StateOutput");
    StateOutput.clock(clock);
    StateOutput.op1(op1);
    StateOutput.op2(op2);
    StateOutput.busy_sel(busy_sel);
    StateOutput.first_cycle(first_cycle);
    StateOutput.preq_sel(preq_sel);
    StateOutput.PENABLE(PENABLE);
    StateOutput.CurrentState_i(CS_StatetoStateOutput);

    //PWRITE & mux:first_cycle and clk already defined

    sc_signal<sc_lv<1>> reg_to_mux("reg_to_mux");
    sc_signal<sc_lv<1>> PWRITE_sig("PWRITE_sig");

    //scPWRITE PWRITE_reg();
    registre<1> PWRITE_reg("MemInt_PWRITE");
    mux2to1 <1> muxPWRITE("muxPWRITE");

    PWRITE_reg.d(wr_i);
    PWRITE_reg.load(first_cycle);
    PWRITE_reg.clock(clock);
    PWRITE_reg.q(reg_to_mux);

    muxPWRITE.i0(reg_to_mux);
    muxPWRITE.i1(wr_i);
    muxPWRITE.sel(first_cycle);
    muxPWRITE.res(PWRITE_sig);

    //ALIGNMENT
    sc_signal<sc_lv<2>> ALIGNMENT_REG_sig("ALIGNEMNT_REG_sig");
    sc_signal<sc_lv<2>> ALIGNMENT_sig("ALIGNMENT_sig");

    registre<2> ALIGNMENT_reg ("MemInt_ALIGNMENT");

    ALIGNMENT_reg.d(ALIGNMENT_sig);
    ALIGNMENT_reg.load(first_cycle);
    ALIGNMENT_reg.clock(clock);
    ALIGNMENT_reg.q(ALIGNMENT_REG_sig);

    //MI_CPU SIDE
    scCPU CPUlink("linkCPUsignals");
    sc_signal<sc_lv<2>> size_i("size_i");
    sc_signal<sc_lv<32>> wdata_i("wdata_i");
    sc_signal<sc_lv<1>>     unsigned_i("unsigned_i");
    CPUlink.size_i(size_i);
    CPUlink.unsigned_i(unsigned_i);
    CPUlink.wdata_i(wdata_i);
    CPUlink.func3(func3);
    CPUlink.rs2_value(rs2_value);

    //---------------------------------------- PSTRB-----------------------------------------------------------
    //SIZESTROBES

    sc_signal<sc_lv<8>> SIZESTRB("SIZESTRB");
    scSizeStrobes sizestrobes ("MemInt_SizeStrobes");

    sizestrobes.size(size_i);
    sizestrobes.strobes(SIZESTRB);

    //shiftleft STRB
    sc_signal<sc_lv<8>> out1tomux("out1tomux");
    sc_signal<sc_lv<8>> out2tomux("out2tomux");
    sc_signal<sc_lv<8>> out3tomux("out3tomux");
    sc_signal<sc_lv<8>> muxtobyte("muxtobyte");

    shiftleft<8,1> shiftleft1 ("MemInt_ShiftLeft1");
    shiftleft1.in(SIZESTRB);
    shiftleft1.out(out1tomux);
    shiftleft<8,2> shiftleft2 ("MemInt_ShiftLeft2");
    shiftleft2.in(SIZESTRB);
    shiftleft2.out(out2tomux);
    shiftleft<8,3> shiftleft3 ("MemInt_ShiftLeft3");
    shiftleft3.in(SIZESTRB);
    shiftleft3.out(out3tomux);

    //MUX4to1 STRB
    mux4to1 <8> muxSTRB("muxSTRB");
    muxSTRB.i0(SIZESTRB);
    muxSTRB.i1(out1tomux);
    muxSTRB.i2(out2tomux);
    muxSTRB.i3(out3tomux);
    muxSTRB.sel(ALIGNMENT_sig);
    muxSTRB.res(muxtobyte);

    //OR BYTESTRB
    sc_signal<sc_lv<1>> OR_to_reg("ORtoREG");
    sc_signal<sc_lv<4>> bytestrb_7to4("bytestrb_7to4");
    scOR OR("MemInt_OR");
    OR.bytestrb_7to4(bytestrb_7to4);
    OR.out(OR_to_reg);

    //register BYTESTRB to mux
    sc_signal<sc_lv<8>> bytestrb("bytestrb");
    sc_signal<sc_lv<8>> reg_bytestrb_output("reg_bytestrb_output");

    registre<8> registre1 ("register_BYTESTRB_to_mux");
    registre1.d(bytestrb);
    registre1.q(reg_bytestrb_output);
    registre1.load(first_cycle);
    registre1.clock(clock);

    //register OR to unaligned
    registre<1> registre2 ("register_OR_to_unaligned");
    registre2.d(OR_to_reg);
    registre2.q(unaligned);
    registre2.load(first_cycle);
    registre2.clock(clock);

    //MUX2to1 BYTESTRB op2
    sc_signal<sc_lv<4>>     reg_bytestrb_output_3to0("reg_bytestrb_output_3to0");
    sc_signal<sc_lv<4>>     reg_bytestrb_output_7to4("reg_bytestrb_output_7to4");
    sc_signal<sc_lv<4>>     muxbytestrb_to_PSTRB("muxbytestrb_to_PSTRB");

    mux2to1<4>  muxop2("muxop2_bytestrb");
    muxop2.i0(reg_bytestrb_output_3to0);
    muxop2.i1(reg_bytestrb_output_7to4);
    muxop2.sel(op2);
    muxop2.res(muxbytestrb_to_PSTRB);

    //MUX2to1 BYTESTRB first_cycle
    sc_signal<sc_lv<4>> bytestrb_3to0("bytestrb_3to0");

    mux2to1<4>  muxfirst_cycle("muxfirst_cycle");
    muxfirst_cycle.i0(muxbytestrb_to_PSTRB);
    muxfirst_cycle.i1(bytestrb_3to0);
    muxfirst_cycle.sel(first_cycle);
    muxfirst_cycle.res(PSTRB);

    //---------------------------------------- PWDATA-----------------------------------------------------------

    //ZEROEXTEND
    scZeroextend zeroext ("MemInt_zeroextend");
    sc_signal<sc_lv<64>> zerotomux("zerotomux");

    zeroext.in(wdata_i);
    zeroext.out(zerotomux);

    //shiftleft PWDATA
    sc_signal<sc_lv<64>> out8tomux("out8tomux");
    sc_signal<sc_lv<64>> out16tomux("out16tomux");
    sc_signal<sc_lv<64>> out24tomux("out24tomux");
    sc_signal<sc_lv<64>> muxtoWdata("muxtoWdata");

    shiftleft<64,8> shiftleft8 ("MemInt_ShiftLeft8");
    shiftleft8.in(zerotomux);
    shiftleft8.out(out8tomux);
    shiftleft<64,16> shiftleft16 ("MemInt_ShiftLeft16");
    shiftleft16.in(zerotomux);
    shiftleft16.out(out16tomux);
    shiftleft<64,24> shiftleft24 ("MemInt_ShiftLeft24");
    shiftleft24.in(zerotomux);
    shiftleft24.out(out24tomux);

    //MUX4to1 WDATA
    mux4to1 <64> muxWDATA("muxWDATA");
    muxWDATA.i0(zerotomux);
    muxWDATA.i1(out8tomux);
    muxWDATA.i2(out16tomux);
    muxWDATA.i3(out24tomux);
    muxWDATA.sel(ALIGNMENT_sig);
    muxWDATA.res(muxtoWdata);

    //register WDATA64 to mux op2
    registre<64> regWdata ("MemInt_registreWdata");
    sc_signal<sc_lv<64>>     reg_to_mux64("reg_to_mux64");
    sc_signal<sc_lv<64>>     WDATA64("WDATA64");
    regWdata.d(WDATA64);
    regWdata.q(reg_to_mux64);
    regWdata.load(first_cycle);
    regWdata.clock(clock);

    //mux2to1 op2 WDATA
    mux2to1<32> muxwdata("mux2to1_wdata");
    sc_signal<sc_lv<32>>     regtomux64_31to0("regtomux64_31to0");
    sc_signal<sc_lv<32>>     regtomux_63to32("regtomux_63to32");
    sc_signal<sc_lv<32>>     muxwdata_to_PWDATA("muxwdata_to_PWDATA");
    muxwdata.i0(regtomux64_31to0);
    muxwdata.i1(regtomux_63to32);
    muxwdata.res(muxwdata_to_PWDATA);
    muxwdata.sel(op2);

    //mux first_cycle PWDATA
    mux2to1<32> muxPWDATA("mux2to1_pwdata");
    sc_signal<sc_lv<32>>     wdata_31to0("wdata_31to0");

    muxPWDATA.i0(muxwdata_to_PWDATA);
    muxPWDATA.i1(wdata_31to0);
    muxPWDATA.res(PWDATA);
    muxPWDATA.sel(first_cycle);

    //---------------------------------------- PADDR-----------------------------------------------------------
    //register
    registre<30>    regPADDR("MemInt-PADDRregister");

    sc_signal<sc_lv<30>>    WORDADDR("WORDADDR");
    sc_signal<sc_lv<30>>    regtomuxPADDR("regtomuxPADDR");
    regPADDR.d(WORDADDR);
    regPADDR.q(regtomuxPADDR);
    regPADDR.load(first_cycle);
    regPADDR.clock(clock);

    //plusPADDR
    scPlus plus("plusPADDR");
    sc_signal<sc_lv<30>>    plustomux("plustomux");
    sc_signal<sc_lv<1>>     one("one");
    plus.one(one);
    plus.in_up(regtomuxPADDR);
    plus.out(plustomux);

    //mux op2 PADDR
    mux2to1<30> muxop2PADDR("muxop2PADDR");
    sc_signal<sc_lv<30>>    muxtoPADDR("muxtoPADDR");
    muxop2PADDR.i0(regtomuxPADDR);
    muxop2PADDR.i1(plustomux);
    muxop2PADDR.res(muxtoPADDR);
    muxop2PADDR.sel(op2);

    //mux first_cycle PADDR
    mux2to1<30> muxPADDR("muxpaddr");

    muxPADDR.i0(muxtoPADDR);
    muxPADDR.i1(WORDADDR);
    muxPADDR.sel(first_cycle);
    muxPADDR.res(PADDR30);


    //---------------------------------------- SELBITS-----------------------------------------------------------
    //select bits
    sc_signal<sc_lv<8>>     mux4_to_bytestrb("mux4_to_bytestrb");
    sc_signal<sc_lv<32>>    wdata64_31to0("wdata64_31to0");

    sc_signal<sc_lv<64>>    out8rtomux("out8rtomux");
    sc_signal<sc_lv<64>>    out16rtomux("out16rtomux");
    sc_signal<sc_lv<64>>    out24rtomux("out24rtomux");
    sc_signal<sc_lv<64>>    RDATA64("RDATA64");
    sc_signal<sc_lv<32>>    RDATA64_31to0("RDATA64_31to0");
    sc_signal<sc_lv<64>>    RDATA64A("RDATA64A");
    sc_signal<sc_lv<64>>    RDATA64B("RDATA64B");
    sc_signal<sc_lv<32>>    regtoRDATA64B("regtoRDATA64B");
    sc_signal<sc_lv<32>>    out8_31to0("out8_31to0");
    sc_signal<sc_lv<32>>    out16_31to0("out16_31to0");
    sc_signal<sc_lv<32>>    out24_31to0("out24_31to0");
    sc_signal<sc_lv<32>>    muxtoextends("muxtoextends");
    sc_signal<sc_lv<16>>    muxtoextends_15to0("muxtoextends_15to0");
    sc_signal<sc_lv<8>>     muxtoextends_7to0("muxtoextends_7to0");

    scSel selbits("MemInt_SelBits");

    selbits.bytestrb(mux4_to_bytestrb);
    selbits.bytestrb_3to0(bytestrb_3to0);
    selbits.bytestrb_7to4(bytestrb_7to4);

    selbits.reg_bytestrb_output(reg_bytestrb_output);
    selbits.reg_bytestrb_output_3to0(reg_bytestrb_output_3to0);
    selbits.reg_bytestrb_output_7to4(reg_bytestrb_output_7to4);

    selbits.wdata64(WDATA64);
    selbits.wdata64_31to0(wdata64_31to0);

    selbits.addr_i(addr_i);
    selbits.wordaddr(WORDADDR);
    selbits.alignment(ALIGNMENT_sig);

    selbits.rdata64b(RDATA64B);
    selbits.rdata64b_31to0(regtoRDATA64B);
    selbits.rdata64b_63to32(PRDATA);

    selbits.in0(PRDATA);
    selbits.out0(rdata_unbuff_o);

    selbits.in1(RDATA64);
    selbits.in2(out8rtomux);
    selbits.in3(out16rtomux);
    selbits.in4(out24tomux);
    selbits.out1(RDATA64_31to0);
    selbits.out2(out8_31to0);
    selbits.out3(out16_31to0);
    selbits.out4(out24_31to0);

    selbits.in5(muxtoextends);
    selbits.out5(muxtoextends_7to0);
    selbits.out6(muxtoextends_15to0);

    selbits.paddr30(PADDR30);
    selbits.paddr32(PADDR32);

    //---------------------------------------- OR TRIGGER-----------------------------------------------------------
    scORTRG ortrg ("MemInt_ORTRG");

    ortrg.rd_i(rd_i);
    ortrg.wr_i(wr_i);
    ortrg.trigger(trigger);

    //---------------------------------------- mux PREQ -----------------------------------------------------------
    mux2to1<1>  muxPREQ ("MemInt_muxPREQ");
    muxPREQ.i0(trigger);
    muxPREQ.i1(one);
    muxPREQ.sel(preq_sel);
    muxPREQ.res(PREQ);

    //---------------------------------------- mux busy_o -----------------------------------------------------------
    mux4to1<1> muxbusy_o("MemInt_Busy_o");
    //sc_signal<sc_lv<1>> busy_o("busy_o");

    muxbusy_o.i0(trigger);
    muxbusy_o.i1(unaligned_or_notPREADY);
    muxbusy_o.i2(one);
    muxbusy_o.i3(notPREADY);
    muxbusy_o.sel(busy_sel);
    muxbusy_o.res(memBusy);

    //---------------------------------------- PRDATA& rdata_o -----------------------------------------------------------
    //zeroextend1
    scZeroextend zeroext1 ("MemInt_Zeroext1");
    zeroext1.in(PRDATA);
    zeroext1.out(RDATA64A);

    //muxRDATA64
    mux2to1<64> muxRDATA64("MemInt_muxRdata64");
    muxRDATA64.i0(RDATA64A);
    muxRDATA64.i1(RDATA64B);
    muxRDATA64.res(RDATA64);
    muxRDATA64.sel(op2);

    //register RDATA64B
    registre<32>  regRDATA64B ("MemInt_registreRDATA64B");
    sc_signal<sc_lv<1>>     and_out("and_out");

    regRDATA64B.d(PRDATA);
    regRDATA64B.q(regtoRDATA64B);
    regRDATA64B.load(and_out);
    regRDATA64B.clock(clock);

    //AND op1 & PREADY
    scAND AND("MemInt_AND_PRDATA");
    AND.op1(op1);
    AND.PREADY(PREADY);
    AND.out(and_out);

    //Shiftright

    shiftright<64,8> shiftright8 ("MemInt_Shiftright8");
    shiftright8.in(RDATA64);
    shiftright8.out(out8rtomux);
    shiftright<64,16> shiftright16 ("MemInt_Shiftright16");
    shiftright16.in(RDATA64);
    shiftright16.out(out16rtomux);
    shiftright<64,24> shiftright24 ("MemInt_Shiftright24");
    shiftright24.in(RDATA64);
    shiftright24.out(out24rtomux);

    //mux4to1 alignment_reg
    mux4to1<32> muxALIREG("MemInt_muxAligReg");
    muxALIREG.i0(RDATA64_31to0);
    muxALIREG.i1(out8_31to0);
    muxALIREG.i2(out16_31to0);
    muxALIREG.i3(out24_31to0);
    muxALIREG.sel(ALIGNMENT_REG_sig);
    muxALIREG.res(muxtoextends);

    //byte extend
    scByteExtend byteext ("MemInt_ByteExtend");
    sc_signal<sc_lv<32>>    byteext_out("byteextend_out");

    byteext.in(muxtoextends_7to0);
    byteext.out(byteext_out);
    byteext.unsigned1(unsigned_i);

    //half extend
    scHalfExtend halfext ("MemInt_HalfExt");
    sc_signal<sc_lv<32>>     halfext_out("halfext_out");
    halfext.in(muxtoextends_15to0);
    halfext.out(halfext_out);
    halfext.unsigned1(unsigned_i);

    //mux3to1 size_i
    mux3to1<32> muxSIZE ("MemInt_muxsize_i");
    sc_signal<sc_lv<32>>    muxSIZEtoreg("muxSIZEtoreg");
    muxSIZE.i0(byteext_out);
    muxSIZE.i1(halfext_out);
    muxSIZE.i2(muxtoextends);
    muxSIZE.res(muxSIZEtoreg);
    muxSIZE.sel(size_i);

    //registre rdata_o
    registre<32>    reg_rdata_o("MemInt_regRDATA_o");
    reg_rdata_o.d(muxSIZEtoreg);
    reg_rdata_o.q(rdata_o);
    reg_rdata_o.load(PREADY);
    reg_rdata_o.clock(clock);

//---------------------------------------- AMBA -----------------------------------------------------------
scIMEM("test1.hex", "IMEM");

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("generaltest");

     sc_trace(wf,clock,"clock");
     sc_trace(wf,RST,"reset");
     /*sc_trace(wf,trigger,"trigger");
     sc_trace(wf,PREADY,"PREADY");
     sc_trace(wf,unaligned,"unaligned");
     sc_trace(wf,op1,"op1");
     sc_trace(wf,op2,"op2");
     sc_trace(wf,first_cycle,"first_cycle");
     sc_trace(wf,busy_sel,"busy_sel");
     sc_trace(wf,preq_sel,"preq_sel");
     sc_trace(wf,PENABLE,"PENABLE");
     sc_trace(wf,CS_StatetoStateOutput,"CS_StatetoStateOutput");
     sc_trace(wf,condition,"condition");*/

     sc_close_vcd_trace_file(wf);
     return 0;
}
