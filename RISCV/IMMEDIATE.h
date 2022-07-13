#include <systemc.h>
#ifndef IMMEDIATE_H
#define IMMEDIATE_H

SC_MODULE (scIMMEDIATE) {
    sc_out<sc_lv<32>>     I{"I"};
    sc_out<sc_lv<32>>     S{"S"};
    sc_out<sc_lv<32>>     U{"U"};
    sc_out<sc_lv<32>>     B{"B"};
    sc_out<sc_lv<32>>     J{"J"};

    sc_in<sc_lv<32>>   RI{"RI"};

    SC_CTOR(scIMMEDIATE) {
        SC_THREAD(store);
    }
    void store () {
        while(1){
            I.write((RI.read()& 0b11111111111)<<0x14);
            S.write(((RI.read()&0b111111100000)<<0x14)|((RI.read()&0b11111)<<0x7));
            B.write(((RI.read()&0b1000000000000)<<0x13)|((RI.read()&0b11111100000)<<0x14)|((RI.read()&0b11110)<<0x7)|((RI.read()&0b100000000000)>>0x4));
            U.write(RI.read()&0b11111111111111111111000000000000);
            J.write(((RI.read()&0b100000000000000000000)<<0xB)|((RI.read()&11111111110)<<0x14)|((RI.read()&0b100000000000)<<0x9)|(RI.read()&0b11111111000000000000));
            wait();
        }
    }

};

#endif // IMMEDIATE_H
