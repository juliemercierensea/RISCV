#ifndef SELRI_VALUE_H
#define SELRI_VALUE_H
#include <systemc.h>

SC_MODULE (scSelRI) {
    // ---------------------      Ports      ---------------------
    sc_in<sc_lv<32>>    RI{"RI"};
    sc_out<sc_lv<3>>    func3{"func3"};
    sc_out<sc_lv<5>>    rs1{"rs1"};
    sc_out<sc_lv<5>>    rs2{"rs2"};
    sc_out<sc_lv<5>>    rd{"rd"};

    SC_CTOR(scSelRI) {
        SC_THREAD(selection);
    }

    void selection() {

        while(1){
            func3.write((RI.read()&0b111000000000000)>>0xC);
            rs1.write((RI.read()&0b11111000000000000000)>>0xF);
            rs2.write((RI.read()&0b1111100000000000000000000)>>0x14);
            rd.write((RI.read()&0b111110000000)>>0x7);

            wait();
        }
    }

};

#endif // SELRI_VALUE_H
