#ifndef FETCHING_SIZE_H
#define FETCHING_SIZE_H
#include <systemc.h>
#include "trace.h"

SC_MODULE (fetch_size) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<32>>   RI{"RI"};
    sc_in<sc_lv<1>>    fetching{"fetching"};
    sc_out<sc_lv<2>>   size{"size"};

    SC_CTOR(fetch_size) {
        SC_THREAD(detectfetch);
        sensitive << RI;
        sensitive << fetching;
    }

    void detectfetch() {
        while (1){
            if(fetching.read()==1){
                size.write(0b11);
            }
            else{
                size.write((RI.read()&0b11000000000000)>>0xC); //func3_1to0
            }
            wait();
        }
     }
};

#endif // FETCHING_SIZE_H
