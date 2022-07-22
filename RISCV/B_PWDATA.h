#ifndef B_PWDATA_H
#define B_PWDATA_H
#include <systemc.h>
#include "MI_zeroextend.h"
#include "MI_shift_left.h"
#include "mux4to1.h"
#include "MI_registre.h"
#include "mux2to1.h"
#include "trace.h"

SC_MODULE(blocPWDATA){

    sc_in<sc_lv<32>>    wdata_i{"wdata_i"};
    sc_in<sc_lv<2>>     ALIGNMENT{"ALIGNMENT"};
    sc_in<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_in_clk           clock{"clock"};
    sc_in<sc_lv<1>>     op2{"op2"};

    sc_out<sc_lv<32>>   PWDATA{"PWDATA"};

    scZeroextend        zeroext {"MemInt_zeroextend"};
    shiftleft<64,8>     shiftleft8 {"MemInt_ShiftLeft8"};
    shiftleft<64,16>    shiftleft16 {"MemInt_ShiftLeft16"};
    shiftleft<64,24>    shiftleft24 {"MemInt_ShiftLeft24"};
    mux4to1 <64>        muxWDATA{"muxWDATA"};
    registre<64>        regWdata {"MemInt_registreWdata"};
    mux2to1<32>         muxwdata{"mux2to1_wdata"};
    mux2to1<32>         muxPWDATA{"mux2to1_pwdata"};

    sc_signal<sc_lv<64>>    zerotomux{"zerotomux"};
    sc_signal<sc_lv<64>>    out8tomux{"out8tomux"};
    sc_signal<sc_lv<64>>    out16tomux{"out16tomux"};
    sc_signal<sc_lv<64>>    out24tomux{"out24tomux"};
    sc_signal<sc_lv<64>>    regtomux64{"reg_to_mux64"};
    sc_signal<sc_lv<32>>    regtomux64_31to0{"regtomux64_31to0"};
    sc_signal<sc_lv<32>>    regtomux64_63to32{"regtomux_63to32"};
    sc_signal<sc_lv<32>>    muxwdata_to_PWDATA{"muxwdata_to_PDATA"};
    sc_signal<sc_lv<64>>    WDATA64{"WDATA64"};
    sc_signal<sc_lv<32>>    wdata64_31to0{"wdata64_31to0"};


    SC_CTOR(blocPWDATA){
        zeroext.in(wdata_i);
        zeroext.out(zerotomux);

        shiftleft8.in(zerotomux);
        shiftleft8.out(out8tomux);
        shiftleft16.in(zerotomux);
        shiftleft16.out(out16tomux);
        shiftleft24.in(zerotomux);
        shiftleft24.out(out24tomux);

        muxWDATA.i0(zerotomux);
        muxWDATA.i1(out8tomux);
        muxWDATA.i2(out16tomux);
        muxWDATA.i3(out24tomux);
        muxWDATA.sel(ALIGNMENT);
        muxWDATA.res(WDATA64);

        regWdata.d(WDATA64);
        regWdata.q(regtomux64);
        regWdata.load(first_cycle);
        regWdata.clock(clock);

        muxwdata.i0(regtomux64_31to0);
        muxwdata.i1(regtomux64_63to32);
        muxwdata.res(muxwdata_to_PWDATA);
        muxwdata.sel(op2);

        muxPWDATA.i0(muxwdata_to_PWDATA);
        muxPWDATA.i1(wdata64_31to0);
        muxPWDATA.res(PWDATA);
        muxPWDATA.sel(first_cycle);

        SC_THREAD(sel);
        sensitive<<WDATA64;
        sensitive << regtomux64;

        wf= sc_create_vcd_trace_file("itest_BPWDATA");

        sc_trace(wf,clock,"clock");
        sc_trace(wf,wdata_i,"wdata_i");
        sc_trace(wf,ALIGNMENT,"ALIGNMENT");
        sc_trace(wf,first_cycle,"first_cycle");
        sc_trace(wf,op2,"op2");
        sc_trace(wf,PWDATA,"PWDATA");
        sc_trace(wf,zerotomux,"zerotomux");
        sc_trace(wf,out8tomux,"out8tomux");
        sc_trace(wf,out16tomux,"out16tomux");
        sc_trace(wf,out24tomux,"out24tomux");
        sc_trace(wf,regtomux64,"regtomux64");
        sc_trace(wf,regtomux64_31to0,"regtomux64_31to0");
        sc_trace(wf,regtomux64_63to32,"regtomux64_63to32");
        sc_trace(wf,muxwdata_to_PWDATA,"muxwdata_to_PWDATA");
        sc_trace(wf,WDATA64,"WDATA64");
        sc_trace(wf,wdata64_31to0,"wdata64_31to0");
    }
    void sel(){
        while(1){
            wdata64_31to0.write(WDATA64.read().to_uint()& 0b11111111111111111111111111111111);

            regtomux64_63to32.write((regtomux64.read()&0b1111111111111111111111111111111100000000000000000000000000000000)>>0x20);
            regtomux64_31to0.write(regtomux64.read()&0b11111111111111111111111111111111);
            wait();
        }
    }
};

#endif // B_PWDATA_H
