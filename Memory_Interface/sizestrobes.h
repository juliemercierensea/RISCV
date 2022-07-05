#include <systemc.h>
#ifndef SIZESTROBES_H
#define SIZESTROBES_H

SC_MODULE (scSizeStrobes) {
    sc_in<sc_lv<2>>     size;
    sc_out<sc_lv<8>>    strobes;

    SC_CTOR(scSizeStrobes) {
        SC_THREAD(sizestrobe);
        sensitive <<size;
        }

    void sizestrobe() {
        if (size.read()==0b1){
        strobes.write(0b00000001);  //bytes
        }
        else {
            if(size.read()==0b10){
                strobes.write(0b0000011);   //halfwords
            }
            else{
                if(size.read()==0b11){
                    strobes.write(0b00001111); //words
                }
            }
        }
    }
};

#endif // SIZESTROBES_H
