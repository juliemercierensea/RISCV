#ifndef B_RDATA_O_H
#define B_RDATA_O_H
#include <systemc.h>
#include "MI_zeroextend.h"
#include "mux4to1.h"
#include "mux2to1.h"
#include "mux3to1.h"
#include "MI_registre.h"
#include "MI_shift_right.h"
#include "MI_byte_extend.h"
#include "MI_half_extend.h"
#include "MI_AND.h"
#include "trace.h"

SC_MODULE(blocRdata_o){

    sc_in_clk   clock{"clock"};
    sc_in<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_in<sc_lv<1>>     op1{"op1"};
    sc_in<sc_lv<1>>     PREADY{"PREADY"};
    sc_in<sc_lv<1>>     op2{"op2"};
    sc_in<sc_lv<2>>     ALIGNMENT_REG{"ALIGNMENT_REG"};
    sc_in<sc_lv<1>>     unsigned_i{"unsigned_i"};
    sc_in<sc_lv<2>>     size_i{"size_i"};

    sc_out<sc_lv<32>>   rdata_o{"rdata_o"};
    sc_out<sc_lv<32>>   rdata_unbuff_o{"rdata_unbuff_o"};

    scZeroextend        zeroext1 {"MemInt_Zeroext1"};
    mux2to1<64>         muxRDATA64{"MemInt_muxRdata64"};
    registre<32>        regRDATA64B {"MemInt_registreRDATA64B"};
    scAND               AND{"MemInt_AND_PRDATA"};
    shiftright<64,8>    shiftright8 {"MemInt_Shiftright8"};
    shiftright<64,16>   shiftright16 {"MemInt_Shiftright16"};
    shiftright<64,24>   shiftright24 {"MemInt_Shiftright24"};
    mux4to1<32>         muxALIREG{"MemInt_muxAligReg"};
    scByteExtend        byteext {"MemInt_ByteExtend"};
    scHalfExtend        halfext {"MemInt_HalfExt"};
    mux3to1<32>         muxSIZE {"MemInt_muxsize_i"};
    registre<32>        reg_rdata_o{"MemInt_regRDATA_o"};

    sc_signal<sc_lv<64>>    out8rtomux{"out8rtomux"};
    sc_signal<sc_lv<64>>    out16rtomux{"out16rtomux"};
    sc_signal<sc_lv<64>>    out24rtomux{"out24rtomux"};
    sc_signal<sc_lv<64>>    RDATA64{"RDATA64"};
    sc_signal<sc_lv<32>>    RDATA64_31to0{"RDATA64_31to0"};
    sc_signal<sc_lv<64>>    RDATA64A{"RDATA64A"};
    sc_signal<sc_lv<64>>    RDATA64B{"RDATA64B"};
    sc_signal<sc_lv<32>>    regtoRDATA64B{"regtoRDATA64B"};
    sc_signal<sc_lv<32>>    out8_31to0{"out8_31to0"};
    sc_signal<sc_lv<32>>    out16_31to0{"out16_31to0"};
    sc_signal<sc_lv<32>>    out24_31to0{"out24_31to0"};
    sc_signal<sc_lv<32>>    muxtoextends{"muxtoextends"};
    sc_signal<sc_lv<16>>    muxtoextends_15to0{"muxtoextends_15to0"};
    sc_signal<sc_lv<8>>     muxtoextends_7to0{"muxtoextends_7to0"};
    sc_signal<sc_lv<1>>     and_out{"and_out"};
    sc_signal<sc_lv<32>>    byteext_out{"byteextend_out"};
    sc_signal<sc_lv<32>>    halfext_out{"halfext_out"};
    sc_signal<sc_lv<32>>    muxSIZEtoreg{"muxSIZEtoreg"};

    sc_lv<64> pre{"pre"};

    SC_CTOR(blocRdata_o){
        zeroext1.in(PRDATA);
        zeroext1.out(RDATA64A);

        muxRDATA64.i0(RDATA64A);
        muxRDATA64.i1(RDATA64B);
        muxRDATA64.res(RDATA64);
        muxRDATA64.sel(op2);

        regRDATA64B.d(PRDATA);
        regRDATA64B.q(regtoRDATA64B);
        regRDATA64B.load(and_out);
        regRDATA64B.clock(clock);

        AND.op1(op1);
        AND.PREADY(PREADY);
        AND.out(and_out);

        shiftright8.in(RDATA64);
        shiftright8.out(out8rtomux);
        shiftright16.in(RDATA64);
        shiftright16.out(out16rtomux);
        shiftright24.in(RDATA64);
        shiftright24.out(out24rtomux);

        muxALIREG.i0(RDATA64_31to0);
        muxALIREG.i1(out8_31to0);
        muxALIREG.i2(out16_31to0);
        muxALIREG.i3(out24_31to0);
        muxALIREG.sel(ALIGNMENT_REG);
        muxALIREG.res(muxtoextends);

        byteext.in(muxtoextends_7to0);
        byteext.out(byteext_out);
        byteext.unsigned1(unsigned_i);

        halfext.in(muxtoextends_15to0);
        halfext.out(halfext_out);
        halfext.unsigned1(unsigned_i);

        muxSIZE.i0(byteext_out);
        muxSIZE.i1(halfext_out);
        muxSIZE.i2(muxtoextends);
        muxSIZE.res(muxSIZEtoreg);
        muxSIZE.sel(size_i);

        reg_rdata_o.d(muxSIZEtoreg);
        reg_rdata_o.q(rdata_o);
        reg_rdata_o.load(PREADY);
        reg_rdata_o.clock(clock);

        SC_THREAD(sel);
        sensitive<<PRDATA;
        sensitive<<out8rtomux;
        sensitive<<out16rtomux;
        sensitive<<out24rtomux;
        sensitive<<muxtoextends;
        sensitive<<regtoRDATA64B;

        wf= sc_create_vcd_trace_file("itest_BRDATA_O");

        sc_trace(wf,clock,"clock");
        sc_trace(wf,PRDATA,"PRDATA");
        sc_trace(wf,op1,"op1");
        sc_trace(wf,PREADY,"PREADY");
        sc_trace(wf,op2,"op2");
        sc_trace(wf,ALIGNMENT_REG,"ALIGNMENT_REG");
        sc_trace(wf,unsigned_i,"unsigned_i");
        sc_trace(wf,size_i,"size_i");
        sc_trace(wf,rdata_o,"rdata_o");
        sc_trace(wf,rdata_unbuff_o,"rdata_unbuff_o");
        sc_trace(wf,out8rtomux,"out8rtomux");
        sc_trace(wf,out16rtomux,"out16rtomux");
        sc_trace(wf,out24rtomux,"out24rtomux");
        sc_trace(wf,RDATA64,"RDATA64");
        sc_trace(wf,RDATA64_31to0,"RDATA64_31to0");
        sc_trace(wf,RDATA64A,"RDATA64A");
        sc_trace(wf,RDATA64B,"RDATA64B");
        sc_trace(wf,regtoRDATA64B,"regtoRDATA64B");
        sc_trace(wf,out8_31to0,"out8_31to0");
        sc_trace(wf,out16_31to0,"out16_31to0");
        sc_trace(wf,out24_31to0,"out24_31to0");
        sc_trace(wf,muxtoextends,"muxtoextends");
        sc_trace(wf,muxtoextends_15to0,"muxtoextends_15to0");
        sc_trace(wf,muxtoextends_7to0,"muxtoextends_7to0");
        sc_trace(wf,and_out,"and_out");
        sc_trace(wf,byteext_out,"byteext_out");
        sc_trace(wf,halfext_out,"halfext_out");
        sc_trace(wf,muxSIZEtoreg,"muxSIZEtoreg");
    }
    void sel(){
        while(1){

            RDATA64_31to0.write(RDATA64.read()&0b11111111111111111111111111111111);
            out8_31to0.write(out8rtomux.read()&0b11111111111111111111111111111111);
            out16_31to0.write(out16rtomux.read()&0b11111111111111111111111111111111);
            out24_31to0.write(out24rtomux.read()&0b11111111111111111111111111111111);

            muxtoextends_7to0.write(muxtoextends.read()&0b11111111); //muxtoextend_7to0
            muxtoextends_15to0.write(muxtoextends.read()&0b1111111111111111); //muxtoextend_15to0

            pre=PRDATA.read();
            pre=pre<<0b100000;          //vérifier que ça fait bien l'opération souhaitée
            RDATA64B.write(pre|regtoRDATA64B.read());

            rdata_unbuff_o.write(PRDATA.read());

            wait();
        }
    }
};

#endif // B_RDATA_O_H
