#include<systemc.h>
#ifndef AMBA_MASTER_H
#define AMBA_MASTER_H

SC_MODULE (scMaster) {
    // ---------------------      Ports      ---------------------

    sc_in_clk            PCLK{"clock_i"};
    sc_in<bool>          PRSTn{"PRSTn"}; //active-LOW reset
    sc_in<sc_lv<32>>     PRDATA{"PRDATA"};
    sc_in<sc_lv<1>>      PREADY{"PREADY"};

    sc_out<sc_lv<32>>     PADDR{"PADDR"};
    sc_out<sc_lv<4>>      PSTRB{"PSTRB"};
    sc_out<sc_lv<32>>     PWDATA{"PWDATA"};
    sc_out<sc_lv<1>>      PWRITE{"PWRITE"};
    sc_out<sc_lv<1>>      PENABLE{"PENABLE"};
    sc_out<sc_lv<1>>      PREQ{"PREQ"};


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

#endif // AMBA_MASTER_H
