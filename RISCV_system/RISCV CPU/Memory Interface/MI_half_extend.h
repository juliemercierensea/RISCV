/*!
 * \file MI_half_extend.h
 * \brief extends the input taking into account the fact that it's signed or unsigned. The input is 2 bytes wide and we extend it to make it 4 bytes wide\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 8th 2022\n
 *
 */
#ifndef MI_HALF_EXTEND_H
#define MI_HALF_EXTEND_H
#include <systemc.h>

SC_MODULE (scHalfExtend) {
    sc_in<sc_lv<16>>    in{"in"};
    sc_in<sc_lv<1>>     unsigned1{"unsigned1"};
    sc_out<sc_lv<32>>   out{"out"};
    sc_lv<32>   a=0xFFFF0000;

    SC_CTOR(scHalfExtend) {
        SC_THREAD(extend);
        sensitive <<unsigned1;
        sensitive <<in;
        }

    void extend() {
        while(1){
            if (unsigned1.read()==0b1){
                out.write(in.read());
            }
            else {
                    if((in.read()&0b1000000000000000)==0){
                        out.write(in.read());
                    }
                    else{
                           out.write(a|in.read());
                    }
            }
            wait();
        }
    }
};


#endif // MI_HALF_EXTEND_H
