#ifndef AMBA_H
#define AMBA_H
#include <systemc.h>
#include "RV_1.h"
#include "AMBA_SlaveDecoder.h"
#include "mux2to1.h"
#include "AMBA_Slave_InstructionMem.h"
#include "trace.h"

SC_MODULE(AMBA){

    sc_in_clk   clock{"clock"};
    sc_in<bool>  RST{"reset"};

    RV_1        RV1 {"RV_1"};
    scIMEM      IMEM{"Instruction_Memory"};
    scDecoder   decoder {"Slave_decoder"};
    mux2to1<32> muxPRDATA{"muxPRDATA"};
    mux2to1<1>  muxPREADY{"muxPREADY"};

    sc_signal<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_signal<sc_lv<1>>     PREADY{"PREADY"};
    sc_signal<sc_lv<32>>    PRDATA0{"PRDATA0"};
    sc_signal<sc_lv<1>>     PREADY0{"PREADY0"};
    sc_signal<sc_lv<32>>    PRDATA1{"PRDATA1"};
    sc_signal<sc_lv<1>>     PREADY1{"PREADY1"};
    sc_signal<sc_lv<32>>    PADDR{"PADDR"};
    sc_signal<sc_lv<8>>     PADDR8{"PADDR8"};
    sc_signal<sc_lv<4>>     PSTRB{"PSTRB"};
    sc_signal<sc_lv<32>>    PWDATA{"PWDATA"};
    sc_signal<sc_lv<1>>     PWRITE{"PWRITE"};
    sc_signal<sc_lv<1>>     PENABLE{"PENABLE"};
    sc_signal<sc_lv<1>>     PREQ{"PREQ"};
    sc_signal<sc_lv<2>>     PSEL{"PSEL"};
    sc_signal<sc_lv<1>>     numSlave{"numSlave"};
    sc_signal<sc_lv<8>>     highAPADDR{"highAPADDR"};


    SC_CTOR(AMBA){

        RV1.RST(RST);
        RV1.clock(clock);
        RV1.PRDATA(PRDATA);
        RV1.PREADY(PREADY);
        RV1.PADDR(PADDR);
        RV1.PSTRB(PSTRB);
        RV1.PWDATA(PWDATA);
        RV1.PWRITE(PWRITE);
        RV1.PENABLE(PENABLE);
        RV1.PREQ(PREQ);

        IMEM.PCLK(clock);
        IMEM.PADDR(PADDR8);
        IMEM.PSTRB(PSTRB);
        IMEM.PENABLE(PENABLE);
        IMEM.PSEL(PSEL);
        IMEM.PRDATA(PRDATA0);
        IMEM.PREADY(PREADY0);

        decoder.address(highAPADDR);
        decoder.enable(PREQ);
        decoder.numSlave(numSlave);
        decoder.sel(PSEL);

        muxPRDATA.i0(PRDATA0);
        muxPRDATA.i1(PRDATA1);
        muxPRDATA.sel(numSlave);
        muxPRDATA.res(PRDATA);

        muxPREADY.i0(PREADY0);
        muxPREADY.i1(PREADY1);
        muxPREADY.sel(numSlave);
        muxPREADY.res(PREADY);

        SC_THREAD(select_int_sig);
        sensitive<<PADDR;
    }

    void select_int_sig(){
        while(1){
            PADDR8.write(PADDR.read()&0b11111111);
            highAPADDR.write((PADDR.read()&0b11111111000000000000000000000000)>>0x24);
            wait();
        }
    }
};

#endif // AMBA_H
