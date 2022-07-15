#ifndef B_PSTRB_H
#define B_PSTRB_H
#include <systemc.h>
#include "MI_sizestrobes.h"
#include "MI_shift_left.h"
#include "mux4to1.h"
#include "mux2to1.h"
#include "MI_ORbytestrb.h"
#include "MI_registre.h"

SC_MODULE(blocPSTRB){

    sc_in_clk   clock{"clock"};
    sc_in<sc_lv<2>>     size_i{"size_i"};
    sc_in<sc_lv<2>>     ALIGNMENT {"ALIGNMENT"};
    sc_in<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_in<sc_lv<1>>     op2{"op2"};

    sc_out<sc_lv<4>>    PSTRB{"PSTRB"};
    sc_out<sc_lv<1>>    unaligned{"unaligned"};

    scSizeStrobes   sizestrobes{"sizestrobes"};
    shiftleft<8,1>  shiftleft1 {"shiftleft1"};
    shiftleft<8,2>  shiftleft2 {"shiftleft2"};
    shiftleft<8,3>  shiftleft3 {"shiftleft3"};
    mux4to1<8>      muxSTRB{"muxSTRB"};
    scOR            OR{"MemInt_OR"};
    registre<8>     registre1 {"register_BYTESTRB_to_mux"};
    registre<1>     registre2 {"register_OR_to_unaligned"};
    mux2to1<4>      muxop2{"muxop2_bytestrb"};
    mux2to1<4>      muxfirst_cycle{"muxfirst_cycle"};

    sc_signal<sc_lv<8>> SIZESTRB{"SIZESTRB"};
    sc_signal<sc_lv<8>> out1tomux{"out1tomux"};
    sc_signal<sc_lv<8>> out2tomux{"out2tomux"};
    sc_signal<sc_lv<8>> out3tomux{"out3tomux"};
    sc_signal<sc_lv<8>> muxtobyte{"muxtobyte"};
    sc_signal<sc_lv<1>> OR_to_reg{"ORtoREG"};
    sc_signal<sc_lv<4>> bytestrb_7to4{"bytestrb_7to4"};
    sc_signal<sc_lv<8>> BYTESTRB{"BYTESTRB"};
    sc_signal<sc_lv<8>> reg_bytestrb_output{"reg_bytestrb_output"};
    sc_signal<sc_lv<4>> reg_bytestrb_output_3to0{"reg_bytestrb_output_3to0"};
    sc_signal<sc_lv<4>> reg_bytestrb_output_7to4{"reg_bytestrb_output_7to4"};
    sc_signal<sc_lv<4>> muxbytestrb_to_PSTRB{"muxbytestrb_to_PSTRB"};
    sc_signal<sc_lv<4>> bytestrb_3to0{"bytestrb_3to0"};


    SC_CTOR(blocPSTRB){
        sizestrobes.size(size_i);
        sizestrobes.strobes(SIZESTRB);

        shiftleft1.in(SIZESTRB);
        shiftleft1.out(out1tomux);
        shiftleft2.in(SIZESTRB);
        shiftleft2.out(out2tomux);
        shiftleft3.in(SIZESTRB);
        shiftleft3.out(out3tomux);

        muxSTRB.i0(SIZESTRB);
        muxSTRB.i1(out1tomux);
        muxSTRB.i2(out2tomux);
        muxSTRB.i3(out3tomux);
        muxSTRB.sel(ALIGNMENT);
        muxSTRB.res(muxtobyte);

        OR.bytestrb_7to4(bytestrb_7to4);
        OR.out(OR_to_reg);

        registre1.d(BYTESTRB);
        registre1.q(reg_bytestrb_output);
        registre1.load(first_cycle);
        registre1.clock(clock);

        registre2.d(OR_to_reg);
        registre2.q(unaligned);
        registre2.load(first_cycle);
        registre2.clock(clock);

        muxop2.i0(reg_bytestrb_output_3to0);
        muxop2.i1(reg_bytestrb_output_7to4);
        muxop2.sel(op2);
        muxop2.res(muxbytestrb_to_PSTRB);

        muxfirst_cycle.i0(muxbytestrb_to_PSTRB);
        muxfirst_cycle.i1(bytestrb_3to0);
        muxfirst_cycle.sel(first_cycle);
        muxfirst_cycle.res(PSTRB);

        SC_THREAD(sel);
        sensitive <<BYTESTRB;

    }
    void sel(){
        bytestrb_3to0.write(BYTESTRB.read().to_uint()& 0b1111);
        bytestrb_7to4.write((BYTESTRB.read().to_uint()&0b11110000)>>0x4);

        reg_bytestrb_output_3to0.write((reg_bytestrb_output.read().to_uint())& 0b1111);
        reg_bytestrb_output_7to4.write((reg_bytestrb_output.read().to_uint()& 0b11110000)>>0x4);
    }
};

#endif // B_PSTRB_H
