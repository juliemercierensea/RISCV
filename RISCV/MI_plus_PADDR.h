/*!
 * \file MI_plus_PADDR.h
 * \brief add 1 to the input\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 9th 2022\n
 *
 */
#ifndef MI_PLUS_PADDR_H
#define MI_PLUS_PADDR_H
#include<systemc.h>

SC_MODULE (scPlus) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<30>>    in_up{"in_up"};
    sc_out<sc_lv<30>>   out{"out"};

    SC_CTOR(scPlus) {
        SC_THREAD(operation);
        sensitive <<in_up;
        }

    void operation() {
        while (1){
            out.write(in_up.read().to_uint()+0x1);

        wait();
        }
    }
};


#endif // MI_PLUS_PADDR_H
