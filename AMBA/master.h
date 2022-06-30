#include <systemc.h>
#ifndef MASTER_H
#define MASTER_H

SC_MODULE (scMaster) {
    // ---------------------      Ports      ---------------------

    sc_in_clk            PCLK{"clock_i"};
    sc_in<bool>          PRSTn; //active-LOW reset
    sc_in<sc_lv<32>>     PRDATA;
    sc_in<sc_lv<1>>      PREADY;

    sc_out<sc_lv<32>>     PADDR;
    sc_out<sc_lv<4>>      PSTRB;
    sc_out<sc_lv<32>>     PWDATA;
    sc_out<sc_lv<1>>      PWRITE;
    sc_out<sc_lv<1>>      PENABLE;
    sc_out<sc_lv<1>>      PREQ;


    SC_CTOR(scMaster) {
        SC_THREAD(decode);
        sensitive << PRSTn;
        sensitive << PCLK.pos();
        }

    void decode() {
        while (1){

        wait();
        }
    }
};

#endif // MASTER_H
