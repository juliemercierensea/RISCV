#ifndef B_PWRITE_H
#define B_PWRITE_H
#include <systemc.h>
#include "MI_registre.h"
#include "mux2to1.h"

SC_MODULE(blocPWRITE){

    sc_in<sc_lv<1>> wr_i{"wr_i"};
    sc_in<sc_lv<1>> first_cycle{"first_cycle"};
    sc_in_clk   clock{"clock"};

    sc_out<sc_lv<1>>    PWRITE{"PWRITE"};

    registre<1> PWRITE_reg{"MemInt_PWRITE"};
    mux2to1 <1> muxPWRITE{"muxPWRITE"};

    sc_signal<sc_lv<1>> reg_to_mux{"reg_to_mux"};


    SC_CTOR(blocPWRITE){

        PWRITE_reg.d(wr_i);
        PWRITE_reg.load(first_cycle);
        PWRITE_reg.clock(clock);
        PWRITE_reg.q(reg_to_mux);

        muxPWRITE.i0(reg_to_mux);
        muxPWRITE.i1(wr_i);
        muxPWRITE.sel(first_cycle);
        muxPWRITE.res(PWRITE);

    }
};

#endif // B_PWRITE_H
