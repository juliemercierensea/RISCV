#include <systemc.h>
#include"State.h"
#include "StateOutput.h"
#include "mux2to1.h"
#include "mux3to1.h"
#include "mux4to1.h"
#include "IMEM.h"
#include "MemoryInterface.h"
#include "register.h"
#include "sizestrobes.h"
#include "shiftleft.h"
#include "IHex.h"
#include "selectbits.h"
#include "OR_bytestrb.h"

int sc_main1 (int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    sc_signal<bool>   clock;
    sc_signal<bool> RST("reset");
    sc_signal<sc_lv<4>>     PSTRB;

    scIMEM("test1.hex", "IMEM");

    //State Machine
    sc_signal<bool> trigger;
    sc_signal<bool> PREADY;
    sc_signal<sc_lv<1>> unaligned;
    sc_signal<bool> condition;

    sc_signal<bool> op1;
    sc_signal<sc_lv<1>> op2;
    sc_signal<sc_lv<1>> first_cycle;
    sc_signal<sc_lv<2>> busy_sel;
    sc_signal<bool> preq_sel;
    sc_signal<bool> PENABLE;
    sc_signal<bool> CurrentState;
    sc_signal<sc_lv<2>> CS_StatetoStateOutput;
    sc_signal<sc_lv<2>> ShowCurrentState;

    scState State("AMBA_State");
    scStateOutput StateOutput("AMBA_StateOutput");

    State.clock(clock);
    State.RST(RST);
    State.trigger(trigger);
    State.PREADY(PREADY);
    State.unaligned(unaligned);
    State.condition(condition);
    State.CurrentState(CS_StatetoStateOutput);

    StateOutput.clock(clock);
    StateOutput.op1(op1);
    StateOutput.op2(op2);
    StateOutput.busy_sel(busy_sel);
    StateOutput.first_cycle(first_cycle);
    StateOutput.preq_sel(preq_sel);
    StateOutput.PENABLE(PENABLE);
    StateOutput.CurrentState_i(CS_StatetoStateOutput);

    //PWRITE & mux:first_cycle and clk already defined
    sc_signal<sc_lv<1>> wr_i;
    sc_signal<sc_lv<1>> reg_to_mux;
    sc_signal<sc_lv<1>> PWRITE_sig;

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
    sc_signal<sc_lv<2>> ALIGNMENT_reg_sig;
    sc_signal<sc_lv<2>> ALIGNMENT_sig;

    registre<2> ALIGNMENT_reg ("MemInt_ALIGNMENT");

    ALIGNMENT_reg.d(ALIGNMENT_sig);
    ALIGNMENT_reg.load(first_cycle);
    ALIGNMENT_reg.clock(clock);
    ALIGNMENT_reg.q(ALIGNMENT_reg_sig);

    //SIZESTROBES
    sc_signal<sc_lv<2>> size_i;
    sc_signal<sc_lv<8>> SIZESTRB;

    scSizeStrobes sizestrobes ("MemInt_SizeStrobes");

    sizestrobes.size(size_i);
    sizestrobes.strobes(SIZESTRB);

    //shiftleft (STRB & PWDATA)
    sc_signal<sc_lv<8>> out1tomux;
    sc_signal<sc_lv<8>> out2tomux;
    sc_signal<sc_lv<8>> out3tomux;
    sc_signal<sc_lv<8>> muxtobyte;

    sc_signal<sc_lv<64>> zerotomux;
    sc_signal<sc_lv<64>> out8tomux;
    sc_signal<sc_lv<64>> out16tomux;
    sc_signal<sc_lv<64>> out24tomux;
    sc_signal<sc_lv<64>> muxtoWdata;

    shiftleft<8,1> shiftleft1 ("MemInt_ShiftLeft1");
    shiftleft1.in(SIZESTRB);
    shiftleft1.out(out1tomux);
    shiftleft<8,2> shiftleft2 ("MemInt_ShiftLeft2");
    shiftleft2.in(SIZESTRB);
    shiftleft2.out(out2tomux);
    shiftleft<8,3> shiftleft3 ("MemInt_ShiftLeft3");
    shiftleft3.in(SIZESTRB);
    shiftleft3.out(out3tomux);
    shiftleft<64,8> shiftleft8 ("MemInt_ShiftLeft8");
    shiftleft8.in(zerotomux);
    shiftleft8.out(out8tomux);
    shiftleft<64,16> shiftleft16 ("MemInt_ShiftLeft16");
    shiftleft16.in(zerotomux);
    shiftleft16.out(out16tomux);
    shiftleft<64,24> shiftleft24 ("MemInt_ShiftLeft24");
    shiftleft24.in(zerotomux);
    shiftleft24.out(out24tomux);

    //MUX4to1 STRB
    mux4to1 <8> muxSTRB("muxSTRB");
    muxSTRB.i0(SIZESTRB);
    muxSTRB.i1(out1tomux);
    muxSTRB.i2(out2tomux);
    muxSTRB.i3(out3tomux);
    muxSTRB.sel(ALIGNMENT_sig);
    muxSTRB.res(muxtobyte);

    //OR BYTESTRB
    sc_signal<sc_lv<1>> OR_to_reg;
    sc_signal<sc_lv<4>> bytestrb_7to4;
    scOR OR("MemInt_OR");
    OR.bytestrb_7to4(bytestrb_7to4);
    OR.out(OR_to_reg);

    //MUX4to1 WDATA
    mux4to1 <64> muxWDATA("muxWDATA");
    muxWDATA.i0(zerotomux);
    muxWDATA.i1(out8tomux);
    muxWDATA.i2(out16tomux);
    muxWDATA.i3(out24tomux);
    muxWDATA.sel(ALIGNMENT_sig);
    muxWDATA.res(muxtoWdata);

    //register BYTESTRB to mux
    sc_signal<sc_lv<8>> bytestrb;
    sc_signal<sc_lv<8>> reg_bytestrb_output;

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
    sc_signal<sc_lv<4>>     reg_bytestrb_output_3to0 ;
    sc_signal<sc_lv<4>>     reg_bytestrb_output_7to4;
    sc_signal<sc_lv<4>>     muxop2_to_muxfirst_cycle;

    mux2to1<4>  muxop2("muxop2_bytestrb");
    muxop2.i0(reg_bytestrb_output_3to0);
    muxop2.i1(reg_bytestrb_output_7to4);
    muxop2.sel(op2);
    muxop2.res(muxop2_to_muxfirst_cycle);

    //MUX2to1 BYTESTRB first_cycle
    sc_signal<sc_lv<4>> bytestrb_3to0;

    mux2to1<4>  muxfirst_cycle("muxfirst_cycle");
    muxop2.i0(muxop2_to_muxfirst_cycle);
    muxop2.i1(bytestrb_3to0);
    muxop2.sel(first_cycle);
    muxop2.res(PSTRB);

    //select bits
    sc_signal<sc_lv<8>>     mux4_to_bytestrb;
    sc_signal<sc_lv<64>>     wdata64;
    sc_signal<sc_lv<32>>     wdata64_31to0;

    scSel selbits("MemInt_SelBits");

    selbits.bytestrb(mux4_to_bytestrb);
    selbits.bytestrb_3to0(bytestrb_3to0);
    selbits.bytestrb_7to4(bytestrb_7to4);

    selbits.reg_bytestrb_output(reg_bytestrb_output);
    selbits.reg_bytestrb_output_3to0(reg_bytestrb_output_3to0);
    selbits.reg_bytestrb_output_7to4(reg_bytestrb_output_7to4);

    selbits.wdata64(wdata64);
    selbits.wdata64_31to0(wdata64_31to0);

     sc_start();
     sc_trace_file *wf = sc_create_vcd_trace_file("generaltest");

     sc_trace(wf,clock,"clock");
     sc_trace(wf,RST,"reset");
     sc_trace(wf,trigger,"trigger");
     sc_trace(wf,PREADY,"PREADY");
     sc_trace(wf,unaligned,"unaligned");
     sc_trace(wf,op1,"op1");
     sc_trace(wf,op2,"op2");
     sc_trace(wf,first_cycle,"first_cycle");
     sc_trace(wf,busy_sel,"busy_sel");
     sc_trace(wf,preq_sel,"preq_sel");
     sc_trace(wf,PENABLE,"PENABLE");
     sc_trace(wf,CS_StatetoStateOutput,"CS_StatetoStateOutput");
     sc_trace(wf,condition,"condition");


     sc_close_vcd_trace_file(wf);
     return 0;
}
