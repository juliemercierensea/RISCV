
#ifndef MI_BYTE_EXTEND_H
#define MI_BYTE_EXTEND_H
#include<systemc.h>

SC_MODULE (scByteExtend) {
    sc_in<sc_lv<8>>     in{"in"};
    sc_in<sc_lv<1>>     unsigned1{"unsigned1"};
    sc_out<sc_lv<32>>   out{"out"};
    sc_lv<32>   a=0xFFFFFF00;

    SC_CTOR(scByteExtend) {
        SC_THREAD(extend);
        sensitive <<unsigned1;
        sensitive <<in;
        }

    void extend() {
        while(1){
            if (unsigned1.read()==1){
                out.write(in.read());
            }
            else {
                    if((in.read()&0b10000000)==0){
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

#endif // MI_BYTE_EXTEND_H
