#include<systemc.h>
#ifndef MI_ORTRIGGER_H
#define MI_ORTRIGGER_H

SC_MODULE (scORTRG) {
    // ---------------------      Ports      ---------------------

    sc_in<sc_lv<1>>    rd_i{"rd_i"};
    sc_in<sc_lv<1>>    wr_i{"wr_i"};
    sc_out<sc_lv<1>>   trigger{"trigger"};

    SC_CTOR(scORTRG) {
        SC_THREAD(logicOR);
        }

    void logicOR() {
        while (1){
            trigger.write(rd_i.read()|wr_i.read());

        wait();
        }
    }
};

#endif // MI_ORTRIGGER_H
