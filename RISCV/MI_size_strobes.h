#include <systemc.h>
#ifndef SIZESTROBES_H
#define SIZESTROBES_H

SC_MODULE (scSizeStrobes) {
    sc_in<sc_lv<2>>     size{"size"};
    sc_out<sc_lv<8>>    strobes{"strobes"};

    SC_CTOR(scSizeStrobes) {
        SC_THREAD(sizestrobe);
        sensitive <<size;
        }

    void sizestrobe() {
        while(1){
        if (size.read()==0b0){
        strobes.write(0b00000001);  //bytes
        }
        else {
            if(size.read()==0b1){
                strobes.write(0b0000011);   //halfwords
            }
            else{
                if((size.read()==0b10)||(size.read()==0b11)){
                    strobes.write(0b00001111); //words
                }
            }
        }
            wait();
        }
    }
};

#endif // SIZESTROBES_H
