#include <systemc.h>
#ifndef AMBA_SELBITS_H
#define AMBA_SELBITS_H
#include <systemc.h>
SC_MODULE (scSelAMBA) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>        PADDR{"address"};
    sc_out<sc_lv<8>>        highAbits_PADDR{"highAbits_PADDR"};
    sc_out<sc_lv<8>>        lowXbits_PADDR{"lowXbits_PADDR"};

    sc_in<sc_lv<8>>         reg_bytestrb_output{"reg_bytestrb_output"};
    sc_out<sc_lv<4>>        reg_bytestrb_output_3to0{"reg_bytestrb_output_3to0"};
    sc_out<sc_lv<4>>        reg_bytestrb_output_7to4{"reg_bytestrb_output_7to4"};


    SC_CTOR(scSelAMBA) {
        SC_THREAD(select);
        }

    void select() {
        while (1){


        wait();
        }
    }
};

#endif // AMBA_SELBITS_H
