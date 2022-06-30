#include <systemc.h>
#ifndef SLAVE0_H
#define SLAVE0_H

SC_MODULE (scSlave0) {
    // ---------------------      Ports      ---------------------

    sc_in_clk            PCLK{"clock_i"};
    sc_in<bool>          PRSTn; //active-LOW reset
    sc_in<sc_lv<32>>     PADDR;
    sc_in<sc_lv<4>>      PSTRB;
    sc_in<sc_lv<32>>     PWDATA;
    sc_in<sc_lv<1>>          PWRITE;
    sc_in<sc_lv<1>>          PENABLE;
    sc_in<sc_lv<1>>          PSEL;

    sc_out<sc_lv<32>>     PRDATA;
    sc_out<sc_lv<1>>          PREADY;

    SC_CTOR(scSlave0) {
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

#endif // SLAVE0_H
