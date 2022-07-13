#include<systemc.h>
#ifndef MI_BYTE_EXTEND_H
#define MI_BYTE_EXTEND_H

SC_MODULE (scByteExtend) {
    sc_in<sc_lv<8>>     in{"in"};
    sc_in<sc_lv<1>>     unsigned1{"unsigned1"};
    sc_out<sc_lv<32>>   out{"out"};

    SC_CTOR(scByteExtend) {
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
                    if((in.read()&0b10000000)==1){
                        out.write(0b11111111111111111111111100000000|in.read());
                    }
                    else{
                        if((in.read()&0b10000000)==0){
                            out.write(in.read());
                        }
                    }
                }
            }
            wait();
        }
    }
};

#endif // MI_BYTE_EXTEND_H
