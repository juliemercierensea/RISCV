#ifndef AMBA_SLAVEDECODER_H
#define AMBA_SLAVEDECODER_H
#include <systemc.h>
#include "trace.h"

SC_MODULE (scDecoder) {

    const int A=8;
    const int S=2;
    const int LS=1;
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<8>>     address{"address"}; //A-bit input with the high bits of PADDR from master
    sc_in<sc_lv<1>>     enable{"enable"}; //1-bit input enable (PREQ from master)

    sc_out<sc_lv<1>>    numSlave{"numSlave"}; //LS-bit output with the slave number
    sc_out<sc_lv<2>>    sel{"sel"}; //S-bit output with PSELx signals for the slaves : 2 slaves so 2bits are enough

    sc_lv<8> SApatterns [3]{"SApatterns"};
    sc_lv<8> SAmasks [3]{"SAmask"};
    sc_lv<4> store_add{"store_add"};

    SC_CTOR(scDecoder) {
        SC_THREAD(decode);
        sensitive << address;
        sensitive <<enable;
        }

    void decode() {

        SApatterns [0]=0;
        SApatterns [1]=0b10000000;
        SApatterns [2]=0b10010000;
        SAmasks[0]=0b11111111;
        SAmasks[1]=0b11110000;
        SAmasks[2]=0b11110000;

        //store_add=(address.read().to_uint()& 0b1111)>>0x4);

        while (1){
            int i=0;
            if (enable.read()==1){
                for(i=0;i<S-1;i++){
                    if(((address.read().to_uint() ^ SApatterns[i]) & SAmasks[i])==0){
                        numSlave.write(i);
                    }
                }
                if (address.read()==0){
                    sel.write(0b1);
                }
                else {
                    store_add=(address.read().to_uint()& 0b1111)>>0x4;
                    if(store_add==0x8){
                        sel.write(0b10);
                    }
                    else{
                        store_add=(address.read().to_uint()& 0b1111)>>0x4;
                        if(store_add==0x9){
                            sel.write(0b11);
                        }
                        else{
                            sel.write(0);
                        }
                    }
                }
            }
            else {
                    sel.write(0);
                }
        wait();
        }
    }
};

#endif // AMBA_SLAVEDECODER_H
