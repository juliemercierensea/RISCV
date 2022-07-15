
#ifndef MI_SELECT_BITS_H
#define MI_SELECT_BITS_H
#include <systemc.h>

SC_MODULE (scSel) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>        in0{"in0"};
    sc_in<sc_lv<64>>        in1{"in1"};
    sc_in<sc_lv<64>>        in2{"in2"};
    sc_in<sc_lv<64>>        in3{"in3"};
    sc_in<sc_lv<64>>        in4{"in4"};
    sc_in<sc_lv<32>>        in5{"in5"};

    sc_out<sc_lv<32>>       out0{"out0"};
    sc_out<sc_lv<32>>       out1{"out1"};
    sc_out<sc_lv<32>>       out2{"out2"};
    sc_out<sc_lv<32>>       out3{"out3"};
    sc_out<sc_lv<32>>       out4{"out4"};
    sc_out<sc_lv<16>>       out6{"out6"};
    sc_out<sc_lv<8>>        out5{"out5"};

    SC_CTOR(scSel) {
        SC_THREAD(select);

        }

    void select() {
        while (1){



            out0.write(in0.read()); //rdata_unbuff_o

            out1.write(in1.read()&0b11111111111111111111111111111111);
            out2.write(in2.read()&0b11111111111111111111111111111111);
            out3.write(in3.read()&0b11111111111111111111111111111111);
            out4.write(in4.read()&0b11111111111111111111111111111111);

            out5.write(in5.read()&0b11111111); //muxtoextend_7to0
            out6.write(in5.read()&0b1111111111111111); //muxtoextend_15to0



        wait();
        }
    }
};

#endif // MI_SELECT_BITS_H
