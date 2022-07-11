#include <systemc.h>
#ifndef AMBA_SLAVE1_DATAMEM_H
#define AMBA_SLAVE1_DATAMEM_H

SC_MODULE (scSlave1) {
    // ---------------------      Ports      ---------------------

    sc_in_clk            PCLK{"clock_i"};
    sc_in<bool>          PRSTn{"Preset"}; //active-LOW reset
    sc_in<sc_lv<32>>     PADDR{"PADDR"};
    sc_in<sc_lv<4>>      PSTRB{"PSTRB"};
    sc_in<sc_lv<32>>     PWDATA{"PWDATA"};
    sc_in<sc_lv<1>>      PWRITE{"PWRITE"};
    sc_in<sc_lv<1>>      PENABLE{"PENABLE"};
    sc_in<sc_lv<2>>      PSEL{"PSEL"};

    sc_out<sc_lv<32>>    PRDATA{"PRDATA"};
    sc_out<sc_lv<1>>     PREADY{"PREADY"};

    SC_CTOR(scSlave1) {
        SC_THREAD(decode);
        sensitive << PRSTn;
        sensitive << PCLK.pos();
        }

    void decode() {
        while (1){
            PREADY.write(PENABLE.read());
            if ((PSEL.read()&0b10)==1){
                if((PWRITE.read()==1)&&(PWDATA.read()==1)){

                    //PRDATA.write();
                }

            }

        wait();
        }
    }
};

#endif // AMBA_SLAVE1_DATAMEM_H
