/*!
 * \file B_PWRITE.h
 * \brief Bloc gathering and binding all the elements producing the output PWRITE in the memory interface\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 18th 2022\n
 * This is also where we create the vcd file associated to PWRITE and its internal signals \n
 */
#ifndef B_PWRITE_H
#define B_PWRITE_H
#include <systemc.h>
#include "MI_registre.h"
#include "mux2to1.h"
#include "trace.h"

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

        wf= sc_create_vcd_trace_file("test_PWRITE");

        sc_trace(wf,clock,"clock");
        sc_trace(wf,first_cycle,"first_cycle");
        sc_trace(wf,wr_i,"wr_i");
        sc_trace(wf,PWRITE,"PWRITE");
        sc_trace(wf,reg_to_mux,"regtomux");

    }
};

#endif // B_PWRITE_H
