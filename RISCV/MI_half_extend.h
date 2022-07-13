#include <systemc.h>
#ifndef MI_HALF_EXTEND_H
#define MI_HALF_EXTEND_H

SC_MODULE (scHalfExtend) {
    sc_in<sc_lv<16>>    in{"in"};
    sc_in<sc_lv<1>>     unsigned1{"unsigned1"};
    sc_out<sc_lv<32>>   out{"out"};

    SC_CTOR(scHalfExtend) {
        SC_THREAD(extend);
        sensitive <<unsigned1;
        }

    void extend() {
        while(1){
            if (unsigned1.read()==0b1){
                out.write(in.read());
            }
            else {
                if(unsigned1.read()==0b0){
                    if((in.read()&0b1000000000000000)==1){
                        out.write(0b11111111111111110000000000000000|in.read());
                    }
                    else{
                        if((in.read()&0b1000000000000000)==0){
                            out.write(in.read());
                        }
                    }
                }
            }
            wait();
        }
    }
};

#endif // MI_HALF_EXTEND_H
