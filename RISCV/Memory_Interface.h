#ifndef MEMORY_INTERFACE_H
#define MEMORY_INTERFACE_H

#include <systemc.h>
#include "MI_registre.h"
#include "mux4to1.h"
#include "mux2to1.h"
#include "B_PADDR.h"
#include "B_PSTRB.h"
#include "B_PWDATA.h"
#include "B_PWRITE.h"
#include "B_rdata_o.h"
#include"B_STATEMACHINE.h"
#include"MI_ORtrigger.h"

#include "IHex.h"

SC_MODULE (scMemory_Interface) {

    sc_in_clk           clock{"clock"};
    sc_in<bool>         RST{"reset"};
    sc_in<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_in<sc_lv<1>>     PREADY{"MemInt_PREADY"};
    sc_in<sc_lv<1>>     rd_i{"rd_i"};
    sc_in<sc_lv<1>>     wr_i{"wr_i"};
    sc_in<sc_lv<32>>    addr_i{"addr_i"};
    sc_in<sc_lv<2>>     size_i{"size_i"};
    sc_in<sc_lv<1>>     unsigned_i{"unsigned_i"};
    sc_in<sc_lv<32>>    wdata_i{"wdata_i"};

    sc_out<sc_lv<32>>   PADDR{"PADDR"};
    sc_out<sc_lv<4>>    PSTRB{"PSTRB"};
    sc_out<sc_lv<32>>   PWDATA{"PWDATA"};
    sc_out<sc_lv<1>>    PWRITE{"PWRITE"};
    sc_out<sc_lv<1>>    PENABLE{"PENABLE"};
    sc_out<sc_lv<1>>    PREQ{"PREQ"};
    sc_out<sc_lv<1>>    busy_o{"busy_o"};
    sc_out<sc_lv<32>>   rdata_o{"rdata_o"};
    sc_out<sc_lv<32>>   rdata_unbuff_o{"rdata_unbuff_o"};

    registre<2>     ALIGNMENT_reg {"MemInt_ALIGNMENT"};
    mux4to1<1>      muxbusy_o{"MemInt_Busy_o"};
    mux2to1<1>      muxPREQ {"MemInt_muxPREQ"};
    scORTRG         ortrg {"MemInt_ORTRG"};
    blocPADDR       blocPADDR{"blocPADDR"};
    blocPSTRB       blocPSTRB{"blocPSTRB"};
    blocPWDATA      blocPWDATA{"blocPWDATA"};
    blocPWRITE      blocPWRITE{"blocPWRITE"};
    blocRdata_o     blocRdata_o{"blocRdata_o"};
    blocSTATEMACHINE blocSTATEMACHINE {"blocSTATEMACHINE"};

    sc_signal<sc_lv<1>>     op1{"op1"};
    sc_signal<sc_lv<1>>     op2{"op2"};
    sc_signal<sc_lv<1>>     first_cycle{"first_cycle"};
    sc_signal<sc_lv<2>>     busy_sel{"busy_sel"};
    sc_signal<sc_lv<1>>     preq_sel{"preq_sel"};
    sc_signal<sc_lv<1>>     trigger{"trigger"};
    sc_signal<sc_lv<1>>     unaligned{"unaligned"};
    sc_signal<sc_lv<2>>     ALIGNMENT{"ALIGNMENT"};
    sc_signal<sc_lv<2>>     ALIGNMENT_REG{"ALIGNMENT_REG"};
    sc_signal<sc_lv<1>>     one{"one"};
    sc_signal<sc_lv<1>>     notPREADY{"notPREADY"};
    sc_signal<sc_lv<1>>     unaligned_or_notPREADY{"unaligned_or_notPREADY"};
    sc_signal<sc_lv<30>>    PADDR30{"PADDR30"};

    SC_CTOR(scMemory_Interface){

        ALIGNMENT_reg.d(ALIGNMENT);
        ALIGNMENT_reg.load(first_cycle);
        ALIGNMENT_reg.clock(clock);
        ALIGNMENT_reg.q(ALIGNMENT_REG);

        muxbusy_o.i0(trigger);
        muxbusy_o.i1(unaligned_or_notPREADY);
        muxbusy_o.i2(one);
        muxbusy_o.i3(notPREADY);
        muxbusy_o.sel(busy_sel);
        muxbusy_o.res(busy_o);

        muxPREQ.i0(trigger);
        muxPREQ.i1(one);
        muxPREQ.sel(preq_sel);
        muxPREQ.res(PREQ);

        ortrg.rd_i(rd_i);
        ortrg.wr_i(wr_i);
        ortrg.trigger(trigger);

        blocPADDR.addr_i(addr_i);
        blocPADDR.clock(clock);
        blocPADDR.first_cycle(first_cycle);
        blocPADDR.op2(op2);
        blocPADDR.PADDR30(PADDR30);
        blocPADDR.ALIGNMENT(ALIGNMENT);

        blocPSTRB.clock(clock);
        blocPSTRB.size_i(size_i);
        blocPSTRB.ALIGNMENT(ALIGNMENT);
        blocPSTRB.first_cycle(first_cycle);
        blocPSTRB.op2(op2);
        blocPSTRB.PSTRB(PSTRB);
        blocPSTRB.unaligned(unaligned);

        blocPWDATA.wdata_i(wdata_i);
        blocPWDATA.ALIGNMENT(ALIGNMENT);
        blocPWDATA.first_cycle(first_cycle);
        blocPWDATA.clock(clock);
        blocPWDATA.op2(op2);
        blocPWDATA.PWDATA(PWDATA);

        blocPWRITE.wr_i(wr_i);
        blocPWRITE.first_cycle(first_cycle);
        blocPWRITE.clock(clock);
        blocPWRITE.PWRITE(PWRITE);

        blocRdata_o.clock(clock);
        blocRdata_o.PRDATA(PRDATA);
        blocRdata_o.op1(op1);
        blocRdata_o.PREADY(PREADY);
        blocRdata_o.op2(op2);
        blocRdata_o.ALIGNMENT_REG(ALIGNMENT_REG);
        blocRdata_o.unsigned_i(unsigned_i);
        blocRdata_o.size_i(size_i);
        blocRdata_o.rdata_o(rdata_o);
        blocRdata_o.rdata_unbuff_o(rdata_unbuff_o);

        blocSTATEMACHINE.RST(RST);
        blocSTATEMACHINE.trigger(trigger);
        blocSTATEMACHINE.PREADY(PREADY);
        blocSTATEMACHINE.unaligned(unaligned);
        blocSTATEMACHINE.clock(clock);
        blocSTATEMACHINE.op1(op1);
        blocSTATEMACHINE.op2(op2);
        blocSTATEMACHINE.first_cycle(first_cycle);
        blocSTATEMACHINE.busy_sel(busy_sel);
        blocSTATEMACHINE.preq_sel(preq_sel);
        blocSTATEMACHINE.PENABLE(PENABLE);

        //selbits.paddr30(PADDR30);
        //selbits.paddr32(PADDR);

        SC_THREAD(internalsignals);
        sensitive << PREADY;
        sensitive <<PADDR30;

            }
    void internalsignals(){
        one.write(0x1);
        notPREADY.write(~PREADY.read());
        unaligned_or_notPREADY.write((unaligned.read()|notPREADY.read()));
        PADDR.write(PADDR30.read());
    }

};

#endif // MEMORY_INTERFACE_H
