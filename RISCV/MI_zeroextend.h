/*!
 * \file MI_zeroextend.h
 * \brief extends the input from 32 to 64 bits wide with 0\n\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 5th 2022\n
 */
#ifndef MI_ZEROEXTEND_H
#define MI_ZEROEXTEND_H
#include <systemc.h>

SC_MODULE (scZeroextend) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>    in{"in"};
    sc_out<sc_lv<64>>   out{"out"};

    SC_CTOR(scZeroextend) {
        SC_THREAD(extend);
        sensitive << in;
        }

    void extend() {
        while (1){
            out.write(in.read());
        wait();
        }
    }
};


#endif // MI_ZEROEXTEND_H
