#ifndef IMMEDIATE_H
#define IMMEDIATE_H
#include <systemc.h>

SC_MODULE (scIMMEDIATE) {
    sc_out<sc_lv<32>>     I{"I"};
    sc_out<sc_lv<32>>     S{"S"};
    sc_out<sc_lv<32>>     U{"U"};
    sc_out<sc_lv<32>>     B{"B"};
    sc_out<sc_lv<32>>     J{"J"};

    sc_in<sc_lv<32>>   RI{"RI"};
    sc_lv<1>    RI_31{"RI31"};
    sc_lv<32>   ones=0xFFFFFFFF;

    SC_CTOR(scIMMEDIATE) {
        SC_THREAD(store);
        sensitive<<RI;
    }
    void store () {
        while(1){
            RI_31=(RI.read()&0x80000000)>>0x1F;
            if(RI_31==1){
                I.write(((RI.read()&0b1111111111100000000000000000000)>>0x14)|((RI.read()&0x80000000)>>0x14)|(ones<<0xC));
                S.write(((RI.read()&0x80000000)>>0x14)|((RI.read()&0b01111110000000000000000000000000)>>0x14)|((RI.read()&0b111110000000)>>0x7)|(ones<<0xC));
                B.write(((RI.read()&0x80000000)>>0x14)|((RI.read()&0x80)<<0x3)|((RI.read()&0b01111110000000000000000000000000)>>0x15)|((RI.read()&0b111100000000)>>0x8)|(ones<<0xC));
                U.write(((RI.read()&0x80000000)>>0xC)|((RI.read()&0b1111111111111111110000000000000)>>0xC)|(ones<<0x14));
                J.write(((RI.read()&0x80000000)>>0xC)|((RI.read()&0b100000000000000000000)>>0xA)|((RI.read()&0b11111111000000000000)>>0x1)|((RI.read()&0b1111111111000000000000000000000)>>0x15)|(ones<<0x14));

            }
            else{
                I.write(((RI.read()&0b1111111111100000000000000000000)>>0x14)|((RI.read()&0x80000000)>>0x14));
                S.write(((RI.read()&0x80000000)>>0x14)|((RI.read()&0b01111110000000000000000000000000)>>0x14)|((RI.read()&0b111110000000)>>0x7));
                B.write(((RI.read()&0x80000000)>>0x14)|((RI.read()&0x80)<<0x3)|((RI.read()&0b01111110000000000000000000000000)>>0x15)|((RI.read()&0b111100000000)>>0x8));
                U.write(((RI.read()&0x80000000)>>0xC)|((RI.read()&0b1111111111111111110000000000000)>>0xC));
                J.write(((RI.read()&0x80000000)>>0xC)|((RI.read()&0b100000000000000000000)>>0xA)|((RI.read()&0b11111111000000000000)>>0x1)|((RI.read()&0b1111111111000000000000000000000)>>0x15));
            }

            //I.write((RI.read()& 0b111111111111)<<0x14);
            //S.write(((RI.read()&0b111111100000)<<0x14)|((RI.read()&0b11111)<<0x7));
            //B.write(((RI.read()&0b1000000000000)<<0x13)|((RI.read()&0b11111100000)<<0x14)|((RI.read()&0b11110)<<0x7)|((RI.read()&0b100000000000)>>0x4));
            //U.write(RI.read()&0b11111111111111111111000000000000);
            //J.write(((RI.read()&0b100000000000000000000)<<0xB)|((RI.read()&0b11111111110)<<0x14)|((RI.read()&0b100000000000)<<0x9)|(RI.read()&0b11111111000000000000));
            wait();
        }
    }

};

#endif // IMMEDIATE_H
