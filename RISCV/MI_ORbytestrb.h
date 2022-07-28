/*!
 * \file MI_ORbytestrb.h
 * \brief the output is a 1 if any of the bites of the input is a 1\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 8th 2022\n
 *
 */
#ifndef MI_ORBYTESTRB_H
#define MI_ORBYTESTRB_H
#include <systemc.h>

SC_MODULE (scOR) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<4>>        bytestrb_7to4{"bytestrb_7to4"};

    sc_out<sc_lv<1>>       out{"out"};

    SC_CTOR(scOR) {
        SC_THREAD(logicOR);
        sensitive<<bytestrb_7to4;
        }

    void logicOR() {
        while (1){
            if(bytestrb_7to4.read().to_uint()!=0){
               out.write(1);
            }
               else {
                   out.write(0);
               }
        wait();
        }
    }
};


#endif // MI_ORBYTESTRB_H
