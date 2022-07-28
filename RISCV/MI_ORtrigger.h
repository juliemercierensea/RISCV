/*!
 * \file MI_ORtrigger.h
 * \brief Logical OR between the 2 inputs\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 8th 2022\n
 * \todo we could make it more general to be used in any design (only need to change the binds and the inputs/outputs names)\n
 *
 */
#ifndef MI_ORTRIGGER_H
#define MI_ORTRIGGER_H
#include<systemc.h>
#include "trace.h"

SC_MODULE (scORTRG) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<1>>    rd_i{"rd_i"};
    sc_in<sc_lv<1>>    wr_i{"wr_i"};
    sc_out<sc_lv<1>>   trigger{"trigger"};

    SC_CTOR(scORTRG) {
        SC_THREAD(logicOR);
        sensitive<<rd_i;
        sensitive<<wr_i;
        }

    void logicOR() {
        while (1){
            trigger.write(rd_i.read().to_int()|wr_i.read().to_int());
        wait();
        }
    }
};

#endif // MI_ORTRIGGER_H
