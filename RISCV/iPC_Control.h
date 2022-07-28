/*!
 * \file iPC_Control.h
 * \brief This comes after the control unit and write iPC only if waitMem or memBusy are set to 0, else iPC =0.\n
 * \author Julie Mercier
 * \version 0.1
 * \date  July 25th 2022\n
 */
#ifndef IPC_CONTROL_H
#define IPC_CONTROL_H
#include <systemc.h>

#include "trace.h"

SC_MODULE (iPC_Control) {
    // ---------------------      Ports      ---------------------
    sc_in<sc_lv<1>>     waitMem{"waitMem"};
    sc_in<sc_lv<1>>     memBusy{"memBusy"};
    sc_in<sc_lv<1>>     iPC_i{"iPC_i"};
    sc_out<sc_lv<1>>    iPC_o{"iPC_o"};

    SC_CTOR(iPC_Control) {
        SC_THREAD(control);
        sensitive << waitMem;
        sensitive << memBusy;
        sensitive << iPC_i;
    }

    void control() {
        while (1){
            if((waitMem.read()==0)||(memBusy.read()==0)){
                iPC_o.write(iPC_i.read());
            }
            else {
                iPC_o.write(0);
            }
            wait();
        }
     }
};

#endif // IPC_CONTROL_H
