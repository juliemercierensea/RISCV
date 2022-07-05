#include <systemc.h>
#ifndef MEMORYINTERFACE_H
#define MEMORYINTERFACE_H

SC_MODULE (scMemInterface) {
    sc_in_clk           clock{"clock_i"};
    sc_in<bool>         reset;
    sc_in<sc_lv<32>>    PRDATA;
    sc_in<sc_lv<1>>     PREADY;
    sc_in<sc_lv<1>>     rd_i;
    sc_in<sc_lv<1>>     wr_i;
    sc_in<sc_lv<32>>    addr_i;
    sc_in<sc_lv<2>>     size_i;
    sc_in<bool>         unsigned_i;
    sc_in<sc_lv<32>>    wdata_i;

    sc_out<sc_lv<32>>   PADDR;
    sc_out<sc_lv<4>>    PSTRB;
    sc_out<sc_lv<32>>   PWDATA;
    sc_out<sc_lv<1>>    PWRITE;
    sc_out<sc_lv<1>>    PENABLE;
    sc_out<sc_lv<1>>    PREQ;
    sc_out<sc_lv<1>>    busy_o;
    sc_out<sc_lv<32>>   rdata_o;
    sc_out<sc_lv<32>>   rdata_unbuff_o;

    SC_CTOR(scMemInterface) {
        SC_THREAD(loading);
        sensitive << clock.pos();
        sensitive << reset;
        }

    void loading() {
        while (1){

            wait();
        }
    }
};

#endif // MEMORYINTERFACE_H
