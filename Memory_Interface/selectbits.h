#include <systemc.h>
#ifndef SELECTBITS_H
#define SELECTBITS_H

SC_MODULE (scSel) {
    // ---------------------      Ports      ---------------------


    sc_in<sc_lv<8>>         bytestrb;
    sc_out<sc_lv<4>>        bytestrb_3to0;
    sc_out<sc_lv<4>>        bytestrb_7to4;

    sc_in<sc_lv<8>>         reg_bytestrb_output;
    sc_out<sc_lv<4>>        reg_bytestrb_output_3to0;
    sc_out<sc_lv<4>>        reg_bytestrb_output_7to4;

    sc_in<sc_lv<64>>        wdata64;
    sc_out<sc_lv<32>>       wdata64_31to0;

    SC_CTOR(scSel) {
        SC_THREAD(select);
        sensitive<<bytestrb;
        sensitive<<reg_bytestrb_output;
        sensitive<<wdata64;
        }

    void select() {
        while (1){
            bytestrb_3to0.write(bytestrb.read().to_uint()& 0b1111);
            bytestrb_7to4.write((bytestrb.read().to_uint()&0b11110000)>>0x4);

            reg_bytestrb_output_3to0.write((reg_bytestrb_output.read().to_uint())& 0b1111);
            reg_bytestrb_output_7to4.write((reg_bytestrb_output.read().to_uint()& 0b11110000)>>0x4);

            wdata64_31to0.write(wdata64.read().to_uint()& 0b11111111111111111111111111111111);

        wait();
        }
    }
};

#endif // SELECTBITS_H
