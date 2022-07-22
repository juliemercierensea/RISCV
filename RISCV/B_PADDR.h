#ifndef B_PADDR_H
#define B_PADDR_H
#include <systemc.h>
#include "MI_registre.h"
#include "mux2to1.h"
#include "MI_plus_PADDR.h"
#include "trace.h"

SC_MODULE(blocPADDR){

    sc_in<sc_lv<32>>    addr_i{"addr_i"};
    sc_in_clk           clock{"clock"};
    sc_in<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_in<sc_lv<1>>     op2{"op2"};

    sc_out<sc_lv<30>>   PADDR30{"PADDR30"};
    sc_out<sc_lv<2>>    ALIGNMENT{"ALIGNMENT"};

    registre<30>    regPADDR{"MemInt_PADDRregister"};
    scPlus          plus{"plusPADDR"};
    mux2to1<30>     muxop2PADDR{"muxop2PADDR"};
    mux2to1<30>     muxPADDR{"muxPADDR"};

    sc_signal<sc_lv<30>>    WORDADDR{"WORDADDR"};
    sc_signal<sc_lv<30>>    regtomuxPADDR{"regtomuxPADDR"};
    sc_signal<sc_lv<30>>    plustomux{"plustomux"};
    sc_signal<sc_lv<30>>    muxtoPADDR{"muxtoPADDR"};

    SC_CTOR(blocPADDR){
        regPADDR.d(WORDADDR);
        regPADDR.q(regtomuxPADDR);
        regPADDR.load(first_cycle);
        regPADDR.clock(clock);

        plus.in_up(regtomuxPADDR);
        plus.out(plustomux);

        muxop2PADDR.i0(regtomuxPADDR);
        muxop2PADDR.i1(plustomux);
        muxop2PADDR.res(muxtoPADDR);
        muxop2PADDR.sel(op2);

        muxPADDR.i0(muxtoPADDR);
        muxPADDR.i1(WORDADDR);
        muxPADDR.sel(first_cycle);
        muxPADDR.res(PADDR30);

        wf= sc_create_vcd_trace_file("itest_BPADDR");

        sc_trace(wf,clock,"clock");
        sc_trace(wf,addr_i,"addr_i");
        sc_trace(wf,ALIGNMENT,"ALIGNMENT");
        sc_trace(wf,first_cycle,"first_cycle");
        sc_trace(wf,op2,"op2");
        sc_trace(wf,PADDR30,"PADDR");
        sc_trace(wf,WORDADDR,"WORDADDR");
        sc_trace(wf,regtomuxPADDR,"regtomuxPADDR");
        sc_trace(wf,plustomux,"plustomux");
        sc_trace(wf,muxtoPADDR,"muxtoPADDR");

        SC_THREAD(sel);
        sensitive <<addr_i;

    }
    void sel(){
        while(1){
            WORDADDR.write((addr_i.read().to_int()&0b1111111111111111111111111111111100)>>0x2);
            ALIGNMENT.write(addr_i.read().to_uint()&0b11);
            wait();
        }
    }
};

#endif // B_PADDR_H
